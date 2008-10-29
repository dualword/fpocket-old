#------------------------------------------------------------
# Makefile.
#------------------------------------------------------------

PATH_GSL    = /home/ecniv/gsl/
PATH_OBJ    = obj/
PATH_SRC    = src/
PATH_HEADER = headers/
PATH_BIN    = bin/

FPOCKET     = fpocket
TPOCKET		= tpocket
DPOCKET		= dpocket
MYPROGS		= $(PATH_BIN)$(FPOCKET) $(PATH_BIN)$(TPOCKET) $(PATH_BIN)$(DPOCKET)

CC          = gcc
LINKER		= gcc

CGSL        = -DMD_NOT_USE_GSL -I$(PATH_GSL)include
COS         = -DM_OS_LINUX
CDEBUG		= -DMNO_MEM_DEBUG
CWARN       = -Wall -Wextra -Wwrite-strings -Wstrict-prototypes

CFLAGS      = $(CWARN) $(COS) $(CDEBUG) -pg -g -O2
#$(CGSL)

LGSL        = -L$(PATH_GSL)lib -lgsl -lgslcblas
LFLAGS	    = -fno-underscoring -lm 
#$(LGSL)

#------------------------------------------------------------
# BINARIES OBJECTS 
#------------------------------------------------------------

FPOBJ = $(PATH_OBJ)fpmain.o $(PATH_OBJ)utils.o $(PATH_OBJ)pertable.o $(PATH_OBJ)memhandler.o $(PATH_OBJ)voronoi.o $(PATH_OBJ)sort.o $(PATH_OBJ)calc.o $(PATH_OBJ)writepdb.o $(PATH_OBJ)rpdb.o $(PATH_OBJ)tparams.o $(PATH_OBJ)fparams.o $(PATH_OBJ)pocket.o $(PATH_OBJ)refine.o $(PATH_OBJ)descriptors.o $(PATH_OBJ)cluster.o $(PATH_OBJ)aa.o $(PATH_OBJ)fpocket.o $(PATH_OBJ)write_visu.o  $(PATH_OBJ)fpout.o $(PATH_OBJ)atom.o $(PATH_OBJ)writepocket.o $(PATH_OBJ)voronoi_lst.o

TPOBJ = $(PATH_OBJ)tpmain.o $(PATH_OBJ)utils.o $(PATH_OBJ)pertable.o $(PATH_OBJ)memhandler.o $(PATH_OBJ)voronoi.o $(PATH_OBJ)sort.o $(PATH_OBJ)calc.o $(PATH_OBJ)writepdb.o $(PATH_OBJ)rpdb.o $(PATH_OBJ)tparams.o $(PATH_OBJ)fparams.o $(PATH_OBJ)pocket.o $(PATH_OBJ)refine.o $(PATH_OBJ)tpocket.o  $(PATH_OBJ)descriptors.o $(PATH_OBJ)cluster.o $(PATH_OBJ)aa.o $(PATH_OBJ)fpocket.o $(PATH_OBJ)write_visu.o $(PATH_OBJ)fpout.o $(PATH_OBJ)atom.o $(PATH_OBJ)writepocket.o $(PATH_OBJ)voronoi_lst.o $(PATH_OBJ)neighbor.o

DPOBJ = $(PATH_OBJ)dpmain.o $(PATH_OBJ)dpocket.o $(PATH_OBJ)dparams.o  $(PATH_OBJ)voronoi.o $(PATH_OBJ)sort.o  $(PATH_OBJ)rpdb.o $(PATH_OBJ)descriptors.o $(PATH_OBJ)neighbor.o $(PATH_OBJ)atom.o $(PATH_OBJ)aa.o $(PATH_OBJ)pertable.o $(PATH_OBJ)calc.o $(PATH_OBJ)utils.o $(PATH_OBJ)writepdb.o $(PATH_OBJ)memhandler.o $(PATH_OBJ)pocket.o $(PATH_OBJ)refine.o $(PATH_OBJ)cluster.o $(PATH_OBJ)fparams.o $(PATH_OBJ)voronoi_lst.o

#------------------------------------------------------------
# GENERAL RULES FOR COMPILATION
#------------------------------------------------------------
$(PATH_OBJ)%.o: $(PATH_SRC)%.c $(PATH_HEADER)%.h
	$(CC) $(CFLAGS) -c $< -o $@

#-----------------------------------------------------------
# RULES FOR EXECUTABLES
#-----------------------------------------------------------

myprogs: $(MYPROGS)

$(PATH_BIN)$(FPOCKET): $(FPOBJ)
	$(LINKER) $(LFLAGS) $^ -o $@

$(PATH_BIN)$(TPOCKET): $(TPOBJ)
	$(LINKER) $(LFLAGS) $^ -o $@

$(PATH_BIN)$(DPOCKET): $(DPOBJ)
	$(LINKER) $(LFLAGS) $^ -o $@

#-----------------------------------------------------------
# DEPENDANCES
#----------------------------------------------------------- 

#---------- Files for fpocket

$(PATH_OBJ)fpmain.o: $(PATH_HEADER)fpmain.h $(PATH_HEADER)fpocket.h $(PATH_HEADER)rpdb.h $(PATH_HEADER)fparams.h $(PATH_HEADER)fpout.h $(PATH_HEADER)memhandler.h

$(PATH_OBJ)fpocket.o: $(PATH_HEADER)fpocket.h $(PATH_HEADER)rpdb.h $(PATH_HEADER)voronoi.h $(PATH_HEADER)rpdb.h  $(PATH_HEADER)pocket.h $(PATH_HEADER)cluster.h $(PATH_HEADER)refine.h $(PATH_HEADER)fparams.h $(PATH_HEADER)memhandler.h

$(PATH_OBJ)fpout.o: $(PATH_HEADER)fpout.h $(PATH_HEADER)writepocket.h $(PATH_HEADER)pocket.h $(PATH_HEADER)writepdb.h $(PATH_HEADER)write_visu.h $(PATH_HEADER)utils.h $(PATH_HEADER)fparams.h

$(PATH_OBJ)fparams.o: $(PATH_HEADER)fparams.h $(PATH_HEADER)utils.h $(PATH_HEADER)memhandler.h

#---------- Files for tpocket

$(PATH_OBJ)tpocket.o: $(PATH_HEADER)tpocket.h $(PATH_HEADER)fpocket.h $(PATH_HEADER)neighbor.h $(PATH_HEADER)memhandler.h $(PATH_HEADER)tparams.h

$(PATH_OBJ)tpmain.o: $(PATH_HEADER)tpmain.h $(PATH_HEADER)tparams.h $(PATH_HEADER)tpocket.h $(PATH_HEADER)fpout.h
$(PATH_OBJ)tparams.o: $(PATH_HEADER)tparams.h $(PATH_HEADER)fparams.h $(PATH_HEADER)utils.h $(PATH_HEADER)memhandler.h

#---------- Files for dpocket

$(PATH_OBJ)dpmain.o: $(PATH_HEADER)dpmain.h $(PATH_HEADER)dparams.h $(PATH_HEADER)dpocket.h $(PATH_HEADER)memhandler.h
$(PATH_OBJ)dparams.o: $(PATH_HEADER)dparams.h $(PATH_HEADER)fparams.h $(PATH_HEADER)utils.h $(PATH_HEADER)memhandler.h
$(PATH_OBJ)dpockets.o: $(PATH_HEADER)dpocket.h $(PATH_HEADER)pocket.h $(PATH_HEADER)cluster.h $(PATH_HEADER)refine.h $(PATH_HEADER)descriptors.h $(PATH_HEADER)neighbor.h $(PATH_HEADER)aa.h $(PATH_HEADER)utils.h $(PATH_HEADER)memhandler.h 


#---------- Commun file used by at least 2 of the 3 programs


$(PATH_OBJ)rpdb.o: $(PATH_HEADER)rpdb.h $(PATH_HEADER)atom.h $(PATH_HEADER)pertable.h $(PATH_HEADER)utils.h   $(PATH_HEADER)memhandler.h 

$(PATH_OBJ)writepocket.o: $(PATH_HEADER)writepocket.h $(PATH_HEADER)writepdb.h $(PATH_HEADER)pocket.h $(PATH_HEADER)voronoi.h  $(PATH_HEADER)utils.h

$(PATH_OBJ)descriptors.o: $(PATH_HEADER)descriptors.h $(PATH_HEADER)atom.h $(PATH_HEADER)voronoi.h $(PATH_HEADER)voronoi_lst.h $(PATH_HEADER)calc.h $(PATH_HEADER)aa.h $(PATH_HEADER)utils.h 

$(PATH_OBJ)neighbor.o: $(PATH_HEADER)neighbor.h $(PATH_HEADER)rpdb.h $(PATH_HEADER)voronoi.h $(PATH_HEADER)sort.h $(PATH_HEADER)atom.h $(PATH_HEADER)memhandler.h

$(PATH_OBJ)pocket.o: $(PATH_HEADER)pocket.h $(PATH_HEADER)voronoi_lst.h $(PATH_HEADER)aa.h $(PATH_HEADER)fparams.h $(PATH_HEADER)descriptors.h $(PATH_HEADER)utils.h $(PATH_HEADER)memhandler.h

$(PATH_OBJ)voronoi.o: $(PATH_HEADER)voronoi.h $(PATH_HEADER)rpdb.h $(PATH_HEADER)writepdb.h $(PATH_HEADER)calc.h  $(PATH_HEADER)memhandler.h

$(PATH_OBJ)voronoi_lst.o: $(PATH_HEADER)voronoi_lst.h $(PATH_HEADER)atom.h $(PATH_HEADER)memhandler.h
$(PATH_OBJ)sort.o: $(PATH_HEADER)sort.h $(PATH_HEADER)voronoi.h $(PATH_HEADER)rpdb.h $(PATH_HEADER)memhandler.h
$(PATH_OBJ)writepdb.o: $(PATH_HEADER)writepdb.h $(PATH_HEADER)rpdb.h $(PATH_HEADER)voronoi.h $(PATH_HEADER)pocket.h
$(PATH_OBJ)cluster.o: $(PATH_HEADER)cluster.h $(PATH_HEADER)voronoi.h $(PATH_HEADER)pocket.h $(PATH_HEADER)calc.h
$(PATH_OBJ)refine.o: $(PATH_HEADER)refine.h $(PATH_HEADER)voronoi.h $(PATH_HEADER)fparams.h $(PATH_HEADER)calc.h $(PATH_HEADER)pocket.h
$(PATH_OBJ)utils.o: $(PATH_HEADER)utils.h $(PATH_HEADER)memhandler.h
$(PATH_OBJ)atom.o: $(PATH_HEADER)atom.h $(PATH_HEADER)utils.h
$(PATH_OBJ)write_visu.o: $(PATH_HEADER)write_visu.h $(PATH_HEADER)utils.h
$(PATH_OBJ)memhandler.o: $(PATH_HEADER)memhandler.h
$(PATH_OBJ)aa.o: $(PATH_HEADER)aa.h 
$(PATH_OBJ)pertable.o: $(PATH_HEADER)pertable.h
$(PATH_OBJ)calc.o: $(PATH_HEADER)calc.h


clean:
	rm $(PATH_OBJ)*.o
	rm $(PATH_BIN)*
	rm $(PATH_SRC)*~
	rm $(PATH_HEADER)*~
