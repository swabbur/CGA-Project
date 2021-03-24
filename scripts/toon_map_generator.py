import png

def sample(value, sample_rate):
    return int(value * sample_rate)/sample_rate


values = [[0 for i in range(512*3)] for j in range(512)]
sampler = 4

for i in range(512) :
    for j in range(512):
        light_intensity = sample(1-j/512, sampler)
        values[i][j*3+1] = 0#int(light_intensity*255)
        y_dir = i/512
        values[i][j*3+0] = int(y_dir*255*light_intensity)
        values[i][j*3+2] = int((1-y_dir)*255*light_intensity)

png.from_array(values, 'RGB').save("toon_map.png")