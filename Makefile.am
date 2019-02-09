AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = symbolic generate_test_data

symbolic_SOURCES = symbolic.cpp \
	args.hpp \
	args.cpp \
	individual.hpp \
	individual.cpp \
	fitness.hpp \
	fitness.cpp \
	functions.hpp \
	functions.cpp \
	genetic.hpp \
	genetic.cpp \
	random.hpp \
	random.cpp \
	tree.hpp \
	tree.cpp
symbolic_LDADD = -lstree

generate_test_data_SOURCES = generate_test_data.cpp \
	functions.hpp \
	functions.cpp
generate_test_data_LDADD = -lstree
