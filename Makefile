shaderpg: main.c glad/src/gl.c shader.c texture.c stb_image.c
	clang -lglfw -lGL -lm -Iglad/include -o $@ $^

clean:
	rm shaderpg
