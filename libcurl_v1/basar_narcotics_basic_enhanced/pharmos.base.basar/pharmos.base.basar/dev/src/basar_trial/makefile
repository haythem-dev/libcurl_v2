##############################################################
#                                                            #
#    makefile for basar trial                                #
#                                                            #
##############################################################


debug release clean gccrel gccdbg: FORCE
	cd ./appl    ; $(MAKE) $@;
	cd ./cmnutil ; $(MAKE) $@;
	cd ./dbaspect; $(MAKE) $@;
	cd ./dbsql   ; $(MAKE) $@;
	cd ./login   ; $(MAKE) $@;
	cd ./property; $(MAKE) $@;

FORCE:
