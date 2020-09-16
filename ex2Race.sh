if test ! -f RNumbers
then 
#create the file with numbers
echo 0 > RNumbers
fi

#repeat 100 times for fun
for i in 'seq 1 1000';
do 
#Read and increase las number
n = 'tail -1 RNumbers'
expr $n +1 >>RNumbers
done
