#------------------------------------------------------------
# Makefile.
#------------------------------------------------------------

PATH_GSL    = /home/user/gsl/
PATH_OBJ    = obj/
PATH_SRC    = src/
PATH_HEADER = headers/
PATH_BIN    = bin/

FPOCKET     = fpocket
TPOCKET		= tpocket
DPOCKET		= dpocket
CHECK		= pcheck
MYPROGS		= $(PATH_BIN)$(FPOCKET) $(PATH_BIN)$(TPOCKET) $(PATH_BIN)$(DPOCKET) $(PATH_BIN)$(CHECK)

CC          = gcc
LINKER		= gcc

CGSL        = -DMD_NOT_USE_GSL -I$(PATH_GSL)include
COS         = -DM_OS_LINUX
CDEBUG		= -DMNO_MEM_DEBUG
CWARN       = -Wall -Wextra -Wwrite-strings

CFLAGS      = $(CWARN) $(COS) $(CDEBUG) -pg -g -O2 #$(CGSL)

LGSL        = -L$(PATH_GSL)lib -lgsl -lgslcblas
LFLAGS	    = -fno-underscoring -lm  #$(LGSL)

#------------------------------------------------------------
# BINARIES OBJECTS 
#------------------------------------------------------------

CHOBJ = $(PATH_OBJ)check.o $(PATH_OBJ)psorting.o $(PATH_OBJ)pscoring.o $(PATH_OBJ)utils.o $(PATH_OBJ)pertable.o $(PATH_OBJ)memhandler.o $(PATH_OBJ)voronoi.o $(PATH_OBJ)sort.o $(PATH_OBJ)calc.o $(PATH_OBJ)writepdb.o $(PATH_OBJ)rpdb.o $(PATH_OBJ)tparams.o $(PATH_OBJ)fparams.o $(PATH_OBJ)pocket.o $(PATH_OBJ)refine.o $(PATH_OBJ)descriptors.o $(PATH_OBJ)cluster.o $(PATH_OBJ)aa.o $(PATH_OBJ)fpocket.o $(PATH_OBJ)write_visu.o  $(PATH_OBJ)fpout.o $(PATH_OBJ)atom.o $(PATH_OBJ)writepocket.o $(PATH_OBJ)voronoi_lst.o $(PATH_OBJ)neighbor.o

FPOBJ = $(PATH_OBJ)fpmain.o $(PATH_OBJ)psorting.o $(PATH_OBJ)pscoring.o $(PATH_OBJ)utils.o $(PATH_OBJ)pertable.o $(PATH_OBJ)memhandler.o $(PATH_OBJ)voronoi.o $(PATH_OBJ)sort.o $(PATH_OBJ)calc.o $(PATH_OBJ)writepdb.o $(PATH_OBJ)rpdb.o $(PATH_OBJ)tparams.o $(PATH_OBJ)fparams.o $(PATH_OBJ)pocket.o $(PATH_OBJ)refine.o $(PATH_OBJ)descriptors.o $(PATH_OBJ)cluster.o $(PATH_OBJ)aa.o $(PATH_OBJ)fpocket.o $(PATH_OBJ)write_visu.o  $(PATH_OBJ)fpout.o $(PATH_OBJ)atom.o $(PATH_OBJ)writepocket.o $(PATH_OBJ)voronoi_lst.o

TPOBJ = $(PATH_OBJ)tpmain.o $(PATH_OBJ)psorting.o $(PATH_OBJ)pscoring.o $(PATH_OBJ)utils.o $(PATH_OBJ)pertable.o $(PATH_OBJ)memhandler.o $(PATH_OBJ)voronoi.o $(PATH_OBJ)sort.o $(PATH_OBJ)calc.o $(PATH_OBJ)writepdb.o $(PATH_OBJ)rpdb.o $(PATH_OBJ)tparams.o $(PATH_OBJ)fparams.o $(PATH_OBJ)pocket.o $(PATH_OBJ)refine.o $(PATH_OBJ)tpocket.o  $(PATH_OBJ)descriptors.o $(PATH_OBJ)cluster.o $(PATH_OBJ)aa.o $(PATH_OBJ)fpocket.o $(PATH_OBJ)write_visu.o $(PATH_OBJ)fpout.o $(PATH_OBJ)atom.o $(PATH_OBJ)writepocket.o $(PATH_OBJ)voronoi_lst.o $(PATH_OBJ)neighbor.o

DPOBJ = $(PATH_OBJ)dpmain.o $(PATH_OBJ)psorting.o $(PATH_OBJ)pscoring.o $(PATH_OBJ)dpocket.o $(PATH_OBJ)dparams.o  $(PATH_OBJ)voronoi.o $(PATH_OBJ)sort.o  $(PATH_OBJ)rpdb.o $(PATH_OBJ)descriptors.o $(PATH_OBJ)neighbor.o $(PATH_OBJ)atom.o $(PATH_OBJ)aa.o $(PATH_OBJ)pertable.o $(PATH_OBJ)calc.o $(PATH_OBJ)utils.o $(PATH_OBJ)writepdb.o $(PATH_OBJ)memhandler.o $(PATH_OBJ)pocket.o $(PATH_OBJ)refine.o $(PATH_OBJ)cluster.o $(PATH_OBJ)fparams.o $(PATH_OBJ)voronoi_lst.o

#------------------------------------------------------------
# GENERAL RULES FOR COMPILATION
#------------------------------------------------------------
$(PATH_OBJ)%.o: $(PATH_SRC)%.c $(PATH_HEADER)%.h
	$(CC) $(CFLAGS) -c $< -o $@

#-----------------------------------------------------------
# RULES FOR EXECUTABLES
#-----------------------------------------------------------

myprogs: $(MYPROGS)

$(PATH_BIN)$(CHECK): $(CHOBJ)
	$(LINKER) $(LFLAGS) $^ -o $@

$(PATH_BIN)$(FPOCKET): $(FPOBJ)
	$(LINKER) $(LFLAGS) $^ -o $@

$(PATH_BIN)$(TPOCKET): $(TPOBJ)
	$(LINKER) $(LFLAGS) $^ -o $@

$(PATH_BIN)$(DPOCKET): $(DPOBJ)
	$(LINKER) $(LFLAGS) $^ -o $@

check: 
	./$(PATH_BIN)$(CHECK)

clean:
	rm $(PATH_OBJ)*.o
	rm $(PATH_BIN)*
	rm $(PATH_SRC)*~
	rm $(PATH_HEADER)*~
