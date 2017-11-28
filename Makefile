Wal = make
Dirs = Inf-Stair libs
.PHONY: all clean

all:
	@for d in $(Dirs);\
	do\
		$(Wal) -C $$d;\
	done

clean:
	@for d in $(Dirs);\
        do\
                $(Wal) -C $$d clean;\
        done

