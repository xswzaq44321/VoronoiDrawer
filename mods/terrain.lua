MaxAltitude = 256
MapWidthX = 800
MapWidthY = 600
MamemakiOffset = 100
Noise = {
    Frequency = 0.005,
    -- *optional
    Method = function(map, offset)
        for i,row in ipairs(map) do
            for j,_ in ipairs(row) do
                map[i][j].height = (Noise.getNoise(offset.x + i, offset.y + j) + 1) / 2 * MaxAltitude;
            end
        end
        return map
    end,
    -- Method = nil,
    -- getNoise will be set by C++
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
    -- Method = function(map) return map end,
    Method = nil,
}