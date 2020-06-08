BiomeTypes = {
    {
        Type = 'snow',
        Moisture = {0.50, 1.00},
        Height = {0.75, 1.00},
        Color = {248, 248, 248}
    },
    {
        Type = 'tundra',
        Moisture = {0.33, 0.50},
        Height = {0.75, 1.00},
        Color = {221, 221, 187}
    },
    {
        Type = 'bare',
        Moisture = {0.16, 0.33},
        Height = {0.75, 1.00},
        Color = {187, 187, 187}
    },
    {
        Type = 'scorched',
        Moisture = {0.00, 0.16},
        Height = {0.75, 1.00},
        Color = {153, 153, 153}
    },
    {
        Type = 'taiga',
        Moisture = {0.66, 1.00},
        Height = {0.50, 0.75},
        Color = {204, 212, 187}
    },
    {
        Type = 'shrubland',
        Moisture = {0.33, 0.66},
        Height = {0.50, 0.75},
        Color = {196, 204, 187}
    },
    {
        Type = 'temperate desert',
        Moisture = {0.00, 0.66},
        Height = {0.50, 0.75},
        Color = {228, 232, 202}
    },
    {
        Type = 'temperate rain forest',
        Moisture = {0.83, 1.00},
        Height = {0.25, 0.50},
        Color = {164, 196, 168}
    },
    {
        Type = 'temperate deciduous forest',
        Moisture = {0.50, 0.83},
        Height = {0.25, 0.50},
        Color = {180, 201, 169}
    },
    {
        Type = 'grassland',
        Moisture = {0.16, 0.50},
        Height = {0.25, 0.50},
        Color = {196, 212, 170}
    },
    {
        Type = 'temperate desert',
        Moisture = {0.00, 0.16},
        Height = {0.25, 0.50},
        Color = {228, 232, 202}
    },
    {
        Type = 'tropical rain forest',
        Moisture = {0.66, 1.00},
        Height = {0.00, 0.25},
        Color = {156, 187, 169}
    },
    {
        Type = 'tropical seasonal forest',
        Moisture = {0.33, 0.66},
        Height = {0.00, 0.25},
        Color = {169, 204, 164}
    },
    {
        Type = 'tropical rain forest',
        Moisture = {0.16, 0.33},
        Height = {0.00, 0.25},
        Color = {196, 212, 170}
    },
    {
        Type = 'tropical rain forest',
        Moisture = {0.00, 0.16},
        Height = {0.00, 0.25},
        Color = {233, 221, 199}
    }
}

function setBiome(polygon)
    for _, v in pairs(BiomeTypes) do
        if
            (v.Moisture[1] <= polygon.moisture and v.Moisture[2] > polygon.moisture and v.Height[1] <= polygon.height and
                v.Height[2] > polygon.height)
         then
            return v
        end
    end
end
