#!/bin/bash
if [ "$1" == 'clean' ]; then
    rm -f *~ *.class *.java
    exit 0
fi
jflex calc.flex
byaccj -J calc.y
javac Parser.java
java Parser
