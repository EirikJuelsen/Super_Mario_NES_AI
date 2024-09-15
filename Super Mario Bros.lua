-- Lua script to get frame information and write it to a file

-- Function to get the X and Y positions from memory
function getPosition()
    -- Read the X position from memory (8-bit value at address 0x0005)
    local xScreenPos = memory.readbyte(0x0005)
    
    -- Read the Y position from memory (8-bit value at address 0x0002)
    local yScreenPos = memory.readbyte(0x0002)

    -- Read the Y position from memory (8-bit value at address 0x0002)
    local xLevelPos = memory.readbyte(0x0086)
    
    -- Print the X and Y positions to the console
    -- print("X Level Position: " .. xLevelPos)
    
    -- Return the positions
    return xScreenPos, yScreenPos, xLevelPos
end

function readInputFromCPP()
    local filePath = "C:/Users/eirik/Source/Repos/Super_Mario_NES_AI/input.txt"    
    local file = io.open(filePath, "r")
    if file then
        local command = file:read("*all")
        file:close()

        -- set up player 1
        local P1 = joypad.get(1)

        local inputTable = {}
        if command == "LEFT" then
            P1["P1 Left"] = true
        elseif command == "A" then
            P1["P1 A"] = true
        elseif command == "RIGHT" then
            P1["P1 Right"] = true
        end


        -- Read input from file depending on what was entered
        local inputTable = {}
        if command == "LEFT" then
            inputTable["P1 Left"] = true
        elseif command == "A" then
            inputTable["P1 A"] = true
        elseif command == "RIGHT" then
            inputTable["P1 Right"] = true
        end

    joypad.set(P1)

    --joypad.set(1, inputTable)
    end
end

-- Function to write the frame information and positions to a file
function writeFrameInfoToFile(xScreenPos, yScreenPos, xLevelPos)
    -- Get the current frame count
    local frameCount = emu.framecount()

    -- Open a file in write mode
    local file = io.open("frame_info.txt", "w")
    
    -- Check if the file was opened successfully
    if file then
        -- Write the frame count, X position, and Y position to the file
        file:write("Frame count: " .. tostring(frameCount) .. "\n")
        file:write("X Position on screen: " .. tostring(xScreenPos) .. "\n")
        file:write("Y Position on screen: " .. tostring(yScreenPos) .. "\n")
	    file:write("X Position in level: " .. tostring(yScreenPos) .. "\n")

        -- print("Frame Count: " .. frameCount)
        -- Close the file
        file:close()
        
        -- Optionally print confirmation
        -- print("Frame count and positions written to file.")
    else
        print("Error: Unable to open file.")
    end
end

-- Main loop to continuously get position and write data to file each frame
while true do
    -- Get the X and Y positions
    local xScreenPos, yScreenPos, xLevelPos = getPosition()
    
    -- Write the frame information and positions to the file
    writeFrameInfoToFile(xScreenPos, yScreenPos, xLevelPos)
    
    -- Read input from CPP and execute
    readInputFromCPP()

    -- Advance to the next frame in BizHawk
    emu.frameadvance()
end