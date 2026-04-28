.PHONY:clean all
main:main.cpp
	g++ -o $@ $<
clean:
	rm main
