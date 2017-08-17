@echo off
rem $Id: make.bat,v 1.4 2014/12/26 04:05:20 phil Exp $
rem So I can just type "make" on Windoze...
nmake -f config\win32\ntmsvc.mak %1
