# taikinys: priklausomybes
#	kodas

CXX = g++
CXXFLAGS = -O3 -std=c++20 # O3 vėliavėlė — optimizuoja programą
OBJ_DIR = obj
BIN_DIR = bin
SRC_DIR = src

IVESTIES_DIR = ivesties_failai
ISVESTIES_DIR = isvesties_failai

PROGR_PAVADINIMAS ?= programa

# .o failų sąrašas
OBJS = $(OBJ_DIR)/pagr.o \
	   $(OBJ_DIR)/studentas.o \
       $(OBJ_DIR)/ivestis.o \
       $(OBJ_DIR)/isvestis.o \
       $(OBJ_DIR)/ivesties_pagalb_fjos.o \
       $(OBJ_DIR)/isvesties_pagalb_fjos.o \
       $(OBJ_DIR)/kt_klasiu_metodai.o \
       $(OBJ_DIR)/klaidu_valdymas.o

# Pagrindinis taikinys
$(BIN_DIR)/$(PROGR_PAVADINIMAS): $(OBJS)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(OBJS) -o $(BIN_DIR)/$(PROGR_PAVADINIMAS) $(LDFLAGS)

# taisyklė visiem .o failam
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@if not exist $(IVESTIES_DIR) mkdir $(IVESTIES_DIR)
	@if not exist $(ISVESTIES_DIR) mkdir $(ISVESTIES_DIR)
# -p : nemes klaidos jei aplankas jau yra
# @ — komandą vykdys tyliai, nerodant jos
# $<, $@ — automatiniai kintamieji
# $< — nurodo pirmąją priklausomybę (taikinys: PRIKLAUSOMYBĖ) (xxx.o: XXX.CPP)
# $@ — nurodo taikinį/tikslą (TAIKINYS: priklausomybė) (XXX.O: xxx.cpp)

# nurodom, kad visi .o failai priklauso nuo visų .h failų
$(OBJS): $(SRC_DIR)/*.h

# ---- testavimas ----

TEST_DIR = testavimas

SRC_TEST = 	$(TEST_DIR)/testavimas.cpp \
			$(SRC_DIR)/studentas.cpp \
			$(SRC_DIR)/ivesties_pagalb_fjos.cpp \
			$(SRC_DIR)/isvesties_pagalb_fjos.cpp \
			$(SRC_DIR)/klaidu_valdymas.cpp

prep_test:
	@if not exist $(TEST_DIR) mkdir $(TEST_DIR)
	g++ $(SRC_TEST) -o $(TEST_DIR)/testavimas

test:
	./$(TEST_DIR)/testavimas

clean:
	del $(OBJ_DIR)\*.o
	del $(BIN_DIR)\*.exe



