MaxAltitude = 256
MapWidthX = 800
MapWidthY = 600
MamemakiOffset = 100
-- range: [-1, 1]
MamemakiThreshold = 0.995
-- range: [-1, 1]
RiverStartAltitude = 0.60
-- range: [-1, 1]
RiverThreshold = 0.9
-- range: [-1, 1]
WaterLayer = 0.3
Noise = {
    Frequency = 0.005,
    -- *optional
    -- signature: table(table)
    Method = function(map)
        for i,row in ipairs(map) do
            for j,_ in ipairs(row) do
                map[i][j].height = (Noise.getNoise(i, j) + 1) / 2 * MaxAltitude;
            end
        end
        return map
    end,
    -- Method = nil,
    -- getNoise function will be set by C++
    getNoise = nil,
}
Smooth = {
    InterpolateMatrix = {
        {0.9, 0.95, 0.9},
        {0.95, 1.0, 0.95},
        {0.9, 0.95, 0.9}
    },
    Iteration = 20,
    -- *optional
    -- Method can be defined as function to use on smoothing terrains
    -- Method signature: table(table)
    Method = nil,
}