#!/usr/bin/lua

--Bins and averages spectral output. 

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

file_input    = "/tmp/Transport_Sampled_Spectrum.process"
file_output   = "/tmp/Transport_Sampled_Spectrum.plot"
numb_of_bins  = 100   --The upper limit on the number of bins.
heading       = " Energy     Relative Intensity (normalized to total output)"
gnuplot       = true --Prints extra points to illustrate bin size, etc. Useful for direct Gnuplot printing.

---------------------------------------------------------------------------------------------------------------------
min_x = 1E99 ; max_x = -1E99 ;       min_y = 1E99 ; max_y = -1E99
xs = Stack:Create() ; ys = Stack:Create()
numb_of_points = 0

io.input(file_input)
--Matches two floating-point, possibly scientific-notation numbers separated or preceeded by space characters.
local pattern = "%s*([%E%.%+%-%e%d]+)%s+([%E%.%+%-%e%d]+)%s+"

for n1, n2 in string.gfind(io.read("*all"), pattern) do
        --print(n1 .. " " .. n2)  --troubleshooting.
        n1 = tonumber(n1)
        n2 = tonumber(n2)
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
output__file:write("#  -- Post analysis: The maximum x value was" .. " " .. max_x .. " " .. "\n")
output__file:write("#  -- Post analysis: The minimum x value was" .. " " .. min_x .. " " .. "\n")
output__file:write("#  -- Post analysis: The maximum y value was" .. " " .. max_y .. " " .. "\n")
output__file:write("#  -- Post analysis: The minimum y value was" .. " " .. min_y .. " " .. "\n")
output__file:write("# " .. heading .. "\n")

--We should have at least, say, 2-particles per bin on average - otherwise we should reduce the number of bins.
if 2*numb_of_points < numb_of_bins then numb_of_bins = 2*numb_of_points end

x_bin_spacing = (max_x - min_x)/numb_of_bins
x_bin_spacing = x_bin_spacing * 1.00001 --To help catch the boundary values.

--Now cycle through the stored data, binning it as we go along.
output = {} --The indices of the output denote x-values (bins), the value denote the number in the bin.

for i=0,(numb_of_bins-1) do 
    output[i] = 0
end

--Produce some tallies.
while true do
    local x = xs:pop(1)
    if not x then break end

    for i=0,(numb_of_bins-1) do
        if( (x >= (min_x + i*x_bin_spacing)) and (x < (min_x + (i+1)*x_bin_spacing))) then
            output[i] = output[i] + 1
            break 
        end
    end 
end 

--Average the output by the count.
for i=0,(numb_of_bins-1) do
    if numb_of_points ~= 0 then  
       output[i] = output[i] / numb_of_points
    end
end

--Now output the data into a directly-plottable form.
for i=0,(numb_of_bins-1) do
    if gnuplot then
        output__file:write(   min_x + (i)*x_bin_spacing .. " " ..       "0" .. " " .. "\n")
        output__file:write(   min_x + (i)*x_bin_spacing .. " " .. output[i] .. " " .. "\n")
        output__file:write( min_x + (i+1)*x_bin_spacing .. " " .. output[i] .. " " .. "\n")
        output__file:write( min_x + (i+1)*x_bin_spacing .. " " ..       "0" .. " " .. "\n")
    else
        if count[i] ~= 0 then --Lets us skip output__file:writeing bins which never held any data.
            output__file:write( min_x + (i+0.5)*x_bin_spacing .. " " .. output[i] .. " " .. "\n")
        end
    end
end

io.close()

