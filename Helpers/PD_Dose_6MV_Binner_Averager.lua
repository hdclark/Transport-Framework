#!/usr/bin/lua

--Bins and numerically integrates dose data over multiple energies to give a (binned) percent depth dose.

--dofile("./Lua_Stack.lua")
----------------------------- A Stack data type. Condensed for portability... -------------------------
Stack = {}
function Stack:Create() -- Create a Table with stack functions
  -- Found at http://lua-users.org/wiki/SimpleStack. "CHILLCODE™"
  local t = {} ; t._et = {} -- stack table ; entry table
  function t:push(...) -- push a value on to the stack
    if ... then local targs = {...} for _,v in pairs(targs) do -- add values
        table.insert(self._et, v) end end end
  function t:pop(num) -- pop a value from the stack
    local num = num or 1 -- get num values from stack
    local entries = {} -- return table
    for i = 1, num do -- get values into entries
      if #self._et ~= 0 then -- get last entry
        table.insert(entries, self._et[#self._et])
        table.remove(self._et) -- remove last value
      else break end end return unpack(entries) end -- return unpacked entries
  function t:getn() return #self._et end -- get entries
  function t:list() for i,v in pairs(self._et) do print(i, v)
    end end  return t  end -- list values
--------------------------------------- end of stack type --------------------------------------------

file_input    = "/tmp/Transport_PD_Dose_6MV.process"
file_output   = "/tmp/Transport_PD_Dose_6MV.plot"
numb_of_x_bins  = 500    -- distance bins.
numb_of_y_bins  = 50    -- energy bins.
heading       = " Distance travelled (cm)    Dose (normalized to maximum) "
gnuplot       = true --Prints extra points to illustrate bin size, etc. Useful for direct Gnuplot printing.

---------------------------------------------------------------------------------------------------------------------
min_x = 1E99 ; max_x = -1E99 ;       min_y = 1E99 ; max_y = -1E99
xs = Stack:Create() ; ys = Stack:Create() ; zs = Stack:Create()
numb_of_points = 0

io.input(file_input)
local pattern = "%s*([%E%.%+%-%e%d]+)%s+([%E%.%+%-%e%d]+)%s+([%E%.%+%-%e%d]+)%s+"

for n1, n2, n3 in string.gfind(io.read("*all"), pattern) do
    if n1 ~= nil and n2 ~= nil and n3 ~= nil and tonumber(n1) ~= nil and tonumber(n2) ~= nil and tonumber(n3) ~= nil then
        n1 = tonumber(n1)
        n2 = tonumber(n2)   
        n3 = tonumber(n3)
        if n1 > max_x then max_x = n1 end
        if n1 < min_x then min_x = n1 end
        if n2 > max_y then max_y = n2 end
        if n2 < min_y then min_y = n2 end

        xs:push(n1)
        ys:push(n2)
        zs:push(n3)
        numb_of_points = numb_of_points + 1
    end
end
io.close() --Close the loaded file.
output__file = io.open(file_output, "w") --Open the output file.

output__file:write("# This file was created by " .. arg[0] .. " using the input file " .. file_input .. " at/on " .. os.date() .. "\n")
output__file:write("#  -- Post analysis: The maximum distance value was" .. " " .. max_x .. " " .. "\n")
output__file:write("#  -- Post analysis: The minimum distance value was" .. " " .. min_x .. " " .. "\n")
output__file:write("#  -- Post analysis: The maximum energy value was" .. " " .. max_y .. " " .. "\n")
output__file:write("#  -- Post analysis: The minimum energy value was" .. " " .. min_y .. " " .. "\n")

x_bin_spacing = (max_x - min_x)/numb_of_x_bins
x_bin_spacing = x_bin_spacing * 1.00001 --To help catch the boundary values.

y_bin_spacing = (max_y - min_y)/numb_of_y_bins
y_bin_spacing = y_bin_spacing * 1.00001 


--Now cycle through the stored data, binning it as we go along.
output = {} 
count  = {} 

--Make the bins.
for i=0,(numb_of_x_bins-1) do 
    output[i] = {}
    count[i]  = {}
    for j=0,(numb_of_y_bins-1) do
        output[i][j] = 0
        count[i][j]  = 0
    end
end


--Produce some tallies.
while true do
    local x = xs:pop(1)   --Get the distance. This lets us find the first bin.
    if not x then break end

    local y = ys:pop(1)   --Get the energy. This lets us find the second bin.
    if not y then break end

    local z = zs:pop(1)   --Get the integration term. 
    if not z then break end

    for i=0,(numb_of_x_bins-1) do
        if( (x >= (min_x + i*x_bin_spacing)) and (x < (min_x + (i+1)*x_bin_spacing))) then

            for j=0,(numb_of_y_bins-1) do
                if( (y >= (min_y + j*y_bin_spacing)) and (y < (min_y + (j+1)*y_bin_spacing))) then

                    output[i][j] = output[i][j] + z*y   --Sort the samples into their proper distance and energy bins.
                    count[i][j]  = count[i][j]  + 1     --Keeping the exact distance and energy is not worthwhile - that is 
                                                        -- what the bins are for. This is also our integration quantization!
                                                        --
                                                        --NOTE: this is basically integrating over each distance,energy bin!
                    break
                end
            end

            break
        end
    end 
end 


--Perform the integration over energy bins (for each distance bin). Note the energy delta is constant and gets cancelled off.
local max_bin = -1E-99
for i=0,(numb_of_x_bins-1) do
    local bin_total = 0
    for j=0,(numb_of_y_bins-1) do
        bin_total = bin_total + output[i][j]
    end

    --Stick the result in output[i][0]
    output[i][0] = bin_total
   
    if bin_total > max_bin then max_bin = bin_total end
end

--Scale the bin to the maximum. This gives us a PDD.
for i=0,(numb_of_x_bins-1) do
    if output[i][0] ~= 0 then  output[i][0] = output[i][0] / max_bin end
end



--Now output the data into a directly-plottable form.
if gnuplot then
    for i=0,(numb_of_x_bins-1) do
        output__file:write( min_x + (i  )*x_bin_spacing .. " ")   output__file:write( "0" .. " ") output__file:write( "\n" ) 
        output__file:write( min_x + (i  )*x_bin_spacing .. " ")   output__file:write( output[i][0] .. " ")  output__file:write( "\n" )
        output__file:write( min_x + (i+1)*x_bin_spacing .. " ")   output__file:write( output[i][0] .. " ")  output__file:write( "\n" )
        output__file:write( min_x + (i+1)*x_bin_spacing .. " ")   output__file:write( "0" .. " ") output__file:write( "\n" )
    end
else
    for i=0,(numb_of_x_bins-1) do
        output__file:write( min_x + (i+0.5)*x_bin_spacing .. " ")  output__file:write( output[i][0] .. " ")  output__file:write( "\n" )
    end
end


io.close()

