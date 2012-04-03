#!/usr/bin/lua

--Bins and averages angular distribution data over multiple energies. 

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

file_input    = "/tmp/Transport_Photon_Angular_Distribution.process"
file_output   = "/tmp/Transport_Photon_Angular_Distribution_phi.plot"
numb_of_x_bins  = 12    -- 12 energy bins.
numb_of_y_bins  = 50    -- 50 angular bins.
heading       = " Phi  Intensity@Energy1  Intensity@Energy2 Intensity@Energy3   ... "
gnuplot       = true --Prints extra points to illustrate bin size, etc. Useful for direct Gnuplot printing.

---------------------------------------------------------------------------------------------------------------------
min_x = 1E99 ; max_x = -1E99 ;       min_y = 1E99 ; max_y = -1E99
xs = Stack:Create() ; ys = Stack:Create()
numb_of_points = 0

io.input(file_input)
--Matches two floating-point, possibly scientific-notation numbers separated or preceeded by space characters.
local pattern = "%s*([%E%.%+%-%e%d]+)%s+([%E%.%+%-%e%d]+)%s+([%E%.%+%-%e%d]+)%s+"

for n1, n2, n3 in string.gfind(io.read("*all"), pattern) do
        --print(n1 .. " " .. n2)  --troubleshooting.
        n1 = tonumber(n1)
        n2 = tonumber(n3)   --  <-----Note we only use the second number here. Switch to n3 if you want to see other angle.
        if n1 > max_x then max_x = n1 end
        if n1 < min_x then min_x = n1 end
        if n2 > max_y then max_y = n2 end
        if n2 < min_y then min_y = n2 end

        xs:push(n1)
        ys:push(n2)
        numb_of_points = numb_of_points + 1
end
io.close() --Close the loaded file.
output__file = io.open(file_output, "w") --Open the output file.

output__file:write("# This file was created by " .. arg[0] .. " using the input file " .. file_input .. " at/on " .. os.date() .. "\n")
output__file:write("#  -- Post analysis: The maximum E value was" .. " " .. max_x .. " " .. "\n")
output__file:write("#  -- Post analysis: The minimum E value was" .. " " .. min_x .. " " .. "\n")
output__file:write("#  -- Post analysis: The maximum phi value was" .. " " .. max_y .. " " .. "\n")
output__file:write("#  -- Post analysis: The minimum phi value was" .. " " .. min_y .. " " .. "\n")

x_bin_spacing = (max_x - min_x)/numb_of_x_bins
x_bin_spacing = x_bin_spacing * 1.00001 --To help catch the boundary values.

y_bin_spacing = (max_y - min_y)/numb_of_y_bins
y_bin_spacing = y_bin_spacing * 1.00001 


--Now cycle through the stored data, binning it as we go along.
output = {} 
count  = {} 

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
    local x = xs:pop(1)   --Get the energy. This lets us find the first bin.
    if not x then break end

    local y = ys:pop(1)   --Get the angle. This lets us find the second bin.
    if not y then break end

    for i=0,(numb_of_x_bins-1) do
        if( (x >= (min_x + i*x_bin_spacing)) and (x < (min_x + (i+1)*x_bin_spacing))) then

            for j=0,(numb_of_y_bins-1) do
                if( (y >= (min_y + j*y_bin_spacing)) and (y < (min_y + (j+1)*y_bin_spacing))) then

                    output[i][j] = output[i][j] + 1     --Sort the samples into their proper energy and angle bins.
                    count[i][j]  = count[i][j]  + 1
                    break
                end
            end

            break
        end
    end 
end 


--Print some info about the binning to file. This is a rather awkward thing. Maybe we should split it into separate files?
output__file:write( "# Energy bin info:  (displayed as ranges)   ")
for i=0,(numb_of_x_bins-1) do
     output__file:write(" ["  .. min_x + (i)*x_bin_spacing .. ":" .. min_x + (i+1)*x_bin_spacing .. "]  ")
end
output__file:write( "\n" )

output__file:write("# " .. heading .. "\n")


--Average the output by the count.
for i=0,(numb_of_x_bins-1) do
    local this_bin_total = 0  --Normalize (over phi) for each energy bin.
    for j=0,(numb_of_y_bins-1) do
        this_bin_total = this_bin_total + output[i][j]
    end

    if this_bin_total ~= 0 then 
        for j=0,(numb_of_y_bins-1) do
            output[i][j] = output[i][j] / this_bin_total
        end 
    end
end



--Now output the data into a directly-plottable form.
if gnuplot then
    for j=0,(numb_of_y_bins-1) do
        output__file:write( min_y + (j  )*y_bin_spacing .. " ")  for i=0,(numb_of_x_bins-1) do  output__file:write( "0" .. " ") end  output__file:write( "\n" )
        output__file:write( min_y + (j  )*y_bin_spacing .. " ")  for i=0,(numb_of_x_bins-1) do  output__file:write( output[i][j] .. " ") end  output__file:write( "\n" )
        output__file:write( min_y + (j+1)*y_bin_spacing .. " ")  for i=0,(numb_of_x_bins-1) do  output__file:write( output[i][j] .. " ") end  output__file:write( "\n" )
        output__file:write( min_y + (j+1)*y_bin_spacing .. " ")  for i=0,(numb_of_x_bins-1) do  output__file:write( "0" .. " ") end  output__file:write( "\n" )

    end
else
    for j=0,(numb_of_y_bins-1) do
        output__file:write( min_y + (j+0.5)*y_bin_spacing .. " ") for i=0,(numb_of_x_bins-1) do output__file:write( output[i][j] .. " ") end output__file:write( "\n" )
    end
end


io.close()

