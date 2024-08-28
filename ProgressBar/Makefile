target=myProgress.exe
src=ProgressBar.c main.c
$(target):$(src)
	gcc $^ -o $@

.PHONY:clean
clean:
	rm -f $(target)
