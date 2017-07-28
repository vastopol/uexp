S3SRC=\
src/AST.java \
src/AstType.java \
src/Call.java \
src/Compiler.java \
src/Constants.java \
src/Debug.java \
src/Define.java \
src/Error.java \
src/EvalStack.java \
src/Frame.java \
src/Function.java \
src/FunctionCompiler.java \
src/Label.java \
src/Lexer.java \
src/Modes.java \
src/Operator.java \
src/Parser.java \
src/Pass1.java \
src/Pass2.java \
src/Pass3.java \
src/PatternOp.java \
src/Primitive.java \
src/Program.java \
src/Snobol3.java \
src/S3Reader.java \
src/Token.java \
src/TokenType.java \
src/Var.java \
src/VM.java

TOOLSRC=\
src/tools/AbstractDebug.java \
src/tools/AbstractDebugPoint.java \
src/tools/CharStream.java \
src/tools/CharStreamSequence.java \
src/tools/Factory.java \
src/tools/Main.java \
src/tools/Parameters.java \
src/tools/ParseArgs.java \
src/tools/Pos.java \
src/tools/QuotedString.java \
src/tools/Ref.java \
src/tools/StringBufferReader.java \
src/tools/Util.java \
src/tools/abstractbody.java \
src/tools/override.java \
src/tools/subclassdefined.java

SRC=${S3SRC} ${TOOLSRC}

CLASSDIR=classes

S3JAR=s3.jar
S3MF=s3.mf
S3MAIN=jsnobol3.Snobol3

classes=${SRC:%.java=%.class}

all: ${S3JAR}

clean::
	rm -fr ${CLASSDIR} ${S3JAR} ${S3MF}

${S3JAR}: cl
	rm -f ${S3MF}
	echo 'Manifest-Version: 1.0' > ${S3MF}
	echo 'Main-Class:' "${S3MAIN}" >> ${S3MF}
	echo '' >> ${S3MF}
	jar -mcf ${S3MF} ${S3JAR} -C ${CLASSDIR} jsnobol3 -C ${CLASSDIR} tools

cl ${CLASSES}: ${SRC} ${CLASSDIR}
	javac -d ${CLASSDIR} -classpath "${CLASSDIR}" ${SRC}

${CLASSDIR}:
	mkdir ${CLASSDIR}

##################################################

TESTS=\
tests/test1 \
tests/test2 \
tests/test3 \
tests/test4 \
tests/test5 \
tests/test6 \
tests/test7 \
tests/test8 \
tests/test9 \
tests/test10

tests::
	for t in  ${TESTS} ; do (\
	    echo "<$$t>" ; \
	    java -jar "${S3JAR}" $$t \
	) done

EXAMPLES=\
examples/example1 \
examples/example2 \
examples/example3 \
examples/example4

examples::
	for t in  ${EXAMPLES} ; do (\
	    echo $$t ; \
	    java -jar "${S3JAR}" $$t \
	) done
