cubeVerts = {
-1.0,-1.0,-1.0,
-1.0,-1.0, 1.0,
-1.0, 1.0, 1.0,
1.0, 1.0,-1.0,
-1.0,-1.0,-1.0,
-1.0, 1.0,-1.0,
1.0,-1.0, 1.0,
-1.0,-1.0,-1.0,
1.0,-1.0,-1.0,
1.0, 1.0,-1.0,
1.0,-1.0,-1.0,
-1.0,-1.0,-1.0,
-1.0,-1.0,-1.0,
-1.0, 1.0, 1.0,
-1.0, 1.0,-1.0,
1.0,-1.0, 1.0,
-1.0,-1.0, 1.0,
-1.0,-1.0,-1.0,
-1.0, 1.0, 1.0,
-1.0,-1.0, 1.0,
1.0,-1.0, 1.0,
1.0, 1.0, 1.0,
1.0,-1.0,-1.0,
1.0, 1.0,-1.0,
1.0,-1.0,-1.0,
1.0, 1.0, 1.0,
1.0,-1.0, 1.0,
1.0, 1.0, 1.0,
1.0, 1.0,-1.0,
-1.0, 1.0,-1.0,
1.0, 1.0, 1.0,
-1.0, 1.0,-1.0,
-1.0, 1.0, 1.0,
1.0, 1.0, 1.0,
-1.0, 1.0, 1.0,
1.0,-1.0, 1.0
};
cubeColors = {
0.583, 0.771, 0.014, 1.0,
0.609, 0.115, 0.436, 1.0,
0.327, 0.483, 0.844, 1.0,
0.822, 0.569, 0.201, 1.0,
0.435, 0.602, 0.223, 1.0,
0.310, 0.747, 0.185, 1.0,
0.597, 0.770, 0.761, 1.0,
0.559, 0.436, 0.730, 1.0,
0.359, 0.583, 0.152, 1.0,
0.483, 0.596, 0.789, 1.0,
0.559, 0.861, 0.639, 1.0,
0.195, 0.548, 0.859, 1.0,
0.014, 0.184, 0.576, 1.0,
0.771, 0.328, 0.970, 1.0,
0.406, 0.615, 0.116, 1.0,
0.676, 0.977, 0.133, 1.0,
0.971, 0.572, 0.833, 1.0,
0.140, 0.616, 0.489, 1.0,
0.997, 0.513, 0.064, 1.0,
0.945, 0.719, 0.592, 1.0,
0.543, 0.021, 0.978, 1.0,
0.279, 0.317, 0.505, 1.0,
0.167, 0.620, 0.077, 1.0,
0.347, 0.857, 0.137, 1.0,
0.055, 0.953, 0.042, 1.0,
0.714, 0.505, 0.345, 1.0,
0.783, 0.290, 0.734, 1.0,
0.722, 0.645, 0.174, 1.0,
0.302, 0.455, 0.848, 1.0,
0.225, 0.587, 0.040, 1.0,
0.517, 0.713, 0.338, 1.0,
0.053, 0.959, 0.120, 1.0,
0.393, 0.621, 0.362, 1.0,
0.673, 0.211, 0.457, 1.0,
0.820, 0.883, 0.371, 1.0,
0.982, 0.099, 0.879, 1.0
}; 

use_3d=true

lua_test=false

rotate = { 0.0,76,45}

eye={ 0.,2.0,7.3}

--(const double fovy, const double aspectRatio, const double zNear, const double zFar) 
projection = {45.0,1.0,-0.1,-100.0}



function init()
    --abs =function (val) if val > 0  then return val else return -val end end
    for k,v in pairs(cubeVerts) do cubeVerts[k]=v*.5; end

    if false then
        for i = 1,#cubeVerts/3 do 
            cubeVerts[3*i]= cubeVerts[3*i] 
        end
    end
end



--[[ here's a simple format for keyboard event 
-- each has three cols
--]]
keyboard = {
    "AD", 0, 0.2, -- eye_x
    -- press A to increase, 
    --S to decrease the value, 
    --initial is 1.5, 
    --step
    "WS", 2 , 0.2, -- eye_y
    "ZX", 7, 0.3, --eye_z
    --here are three lines that only provide
    --key stroke to increase the value
    "J", 0, 5, --rot_x
    "K", 0, 5, --rot_y
    "L", 0, 5, --rot_z
}

--[[ Similarly, 
-- each row means: trigger, initial, change rate ( in s^-1)
--]]
timeVariable = {
    "R",0,45,  -- rot_z, increasing 45 degree each second
}
