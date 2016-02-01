for i in {1..2000}
do
   touch xml/$i.xml
   echo "MT-TESTPERF. -> $i" > xml/$i.xml
done
