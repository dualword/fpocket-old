
# Compiler and linkers:
CFLAGS = -Wall -O2 -Wextra -Wwrite-strings -Wstrict-prototypes
LDFLAGS = -fno-underscoring -lm

# Targets: bin means that programs will be installed in /usr/local/bin
# I guess the use of --prefix PATH when launching ./configure changes
# the prefix path from /usr/local to PATH
bin_PROGRAMS = fpocket tpocket dpocket 

# Dependances
fpocket_SOURCES = fpmain.c psorting.c pscoring.c utils.c pertable.c memhandler.c voronoi.c sort.c calc.c writepdb.c rpdb.c tparams.c fparams.c pocket.c refine.c descriptors.c cluster.c aa.c fpocket.c write_visu.c  fpout.c atom.c writepocket.c voronoi_lst.c  ../headers/fpmain.h ../headers/psorting.h ../headers/pscoring.h ../headers/utils.h ../headers/pertable.h ../headers/memhandler.h ../headers/voronoi.h ../headers/sort.h ../headers/calc.h ../headers/writepdb.h ../headers/rpdb.h ../headers/tparams.h ../headers/fparams.h ../headers/pocket.h ../headers/refine.h ../headers/descriptors.h ../headers/cluster.h ../headers/aa.h ../headers/fpocket.h ../headers/write_visu.h  ../headers/fpout.h ../headers/atom.h ../headers/writepocket.h ../headers/voronoi_lst.h

tpocket_SOURCES = tpmain.c psorting.c pscoring.c utils.c pertable.c memhandler.c voronoi.c sort.c calc.c writepdb.c rpdb.c tparams.c fparams.c pocket.c refine.c tpocket.c  descriptors.c cluster.c aa.c fpocket.c write_visu.c fpout.c atom.c writepocket.c voronoi_lst.c neighbor.c ../headers/tpmain.h ../headers/psorting.h ../headers/pscoring.h ../headers/utils.h ../headers/pertable.h ../headers/memhandler.h ../headers/voronoi.h ../headers/sort.h ../headers/calc.h ../headers/writepdb.h ../headers/rpdb.h ../headers/tparams.h ../headers/fparams.h ../headers/pocket.h ../headers/refine.h ../headers/tpocket.h  ../headers/descriptors.h ../headers/cluster.h ../headers/aa.h ../headers/fpocket.h ../headers/write_visu.h ../headers/fpout.h ../headers/atom.h ../headers/writepocket.h ../headers/voronoi_lst.h ../headers/neighbor.h

dpocket_SOURCES =  dpmain.c psorting.c pscoring.c dpocket.c dparams.c  voronoi.c sort.c  rpdb.c descriptors.c neighbor.c atom.c aa.c pertable.c calc.c utils.c writepdb.c memhandler.c pocket.c refine.c cluster.c fparams.c voronoi_lst.c ../headers/dpmain.h ../headers/psorting.h ../headers/pscoring.h ../headers/dpocket.h ../headers/dparams.h  ../headers/voronoi.h ../headers/sort.h  ../headers/rpdb.h ../headers/descriptors.h ../headers/neighbor.h ../headers/atom.h ../headers/aa.h ../headers/pertable.h ../headers/calc.h ../headers/utils.h ../headers/writepdb.h ../headers/memhandler.h ../headers/pocket.h ../headers/refine.h ../headers/cluster.h ../headers/fparams.h ../headers/voronoi_lst.h
