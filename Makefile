shaderpg: main.c glad/src/gl.c
	clang -lglfw -lGL -Iglad/include -o $@ $^

clean:
	rm shaderpg