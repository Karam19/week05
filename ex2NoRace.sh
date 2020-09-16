#create file with numbers
if test ! -f numbers
then 
echo 0 > numbers
fi
#locks and do not let interrupts
if ln numbers numbers.lock
then
#repeat 100 times for fun
for i in 'seq 1 1000';
do 
n = 'tail -1 numbers'
expr $n +1 >>numbers
done

#unlock
rm numbers.lock
fi