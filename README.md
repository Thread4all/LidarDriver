# contributi al progetto

- (2103037) Mason Eric:
  - Funzioni: get_scan() + test
  - Misc: firma della classe, repl mode, makefile, print_buffer()
- (2101785) Violetto Matteo:
  - Funzioni: get_scan(), get_distance() + test
  - Misc: refactoring vari
- (2109926) Nicolini Pietro:
  - Funzioni: clear_buffer(), operator<< overload + test

# metodi supplementari

LidarDriver::print_buffer() : stampa console tutto il buffer, evidenziando la prossima riga su cui scriverà get_scan e quella più vecchia

# guida cmake

qualora fosse installato, è possibile fare uso di make

se si programma su windows è già a posto, altrimenti occorre sostituire le righe commentate nel makefile _(decommentare r9, 24, 25 e commentare r10, 22, 23)_

compilare:
$ make

eseguire:
$ make run

eseguire in modalità repl:
$ make repl

rimuovere l'eseguibile e i .o:
$ make clean
