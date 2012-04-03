#!/usr/bin/lua

dofile("./Lua_Stack.lua")

--Bins spectral data. Does not handle comments (yet)!

numb_of_bins = 200
min_val = 1E10
max_val = -1E10

data = Stack:Create()

while true do

    --First number is the spectra. Second is irrelevant.
    local n1, n2 = io.read("*number", "*number")
    if not n1 then break end

    if n1 > max_val then max_val = n1 end
    if n1 < min_val then min_val = n1 end

    data:push(n1)

end

print("#Post analysis: The maximum value was " .. max_val)
print("#Post analysis: The minimum value was " .. min_val)

spacing = (max_val - min_val)/numb_of_bins

--Now cycle through the stored data, binning it as we go along.
output = {}
for i=0,(numb_of_bins-1) do 
    output[i] = 0
end

while true do
    
    local n1 = data:pop(1)
    if not n1 then break end

    for i=0,(numb_of_bins-1) do
        if( (n1 >= (min_val + i*spacing)) and (n1 < (min_val + (i+1)*spacing))) then
            output[i] = output[i] + 1
--            break
        end
    end --for loop.

end --while loop.


--Now output the data into a directly-plottable form.
for i, numbof in ipairs(output) do
      print(min_val + i*spacing, numbof)
end
