Wal = make
Dirs = lib Inf-Stair
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

