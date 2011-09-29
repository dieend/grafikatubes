@echo on
if exist mclip.exe
del mclip.exe
tcc -ml mclip clip drawing mouse font point lpoint
mclip
