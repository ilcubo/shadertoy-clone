shaderpg: main.c glad/src/gl.c shader.c
	clang -lglfw -lGL -Iglad/include -o $@ $^

clean:
	rm shaderpg
