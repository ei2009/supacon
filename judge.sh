#! /usr/bin/bash

echo "SuperCon Judge 2021"

CASE_N=103

if [ $HOSTNAME != "cricket" ]; then
    echo "403 not cricket"
    exit
fi

if [ ! -f ./Main.cpp ]; then
    echo "404 file not found './Main.cpp'"
    exit
fi

echo "Compiling..."
g++ -O2 -std=gnu++2a ./Main.cpp -o target.out &> /dev/null
if [ $? != 0 ]; then
    echo -e -n "\033[0;37m\033[0;42m Compile Error \033[0;39m"
    exit
fi

LD_LIBRARY_PATH_TMP=$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/pub/syoribu/lib:$LD_LIBRARY_PATH

ulimit -d 1000000 -m 1000000 -v 1000000
ulimit -s 65532

list_cost=()
runtime=0
totalcost=0
max_time=0
min_time=100
max_cost=0
min_cost=3000
count_yes=0
count_ac=0

for i in `seq -w 1 $CASE_N`
do
    echo
    echo
    echo "[TestCase $i]"
    nowtime=$( ( time ( (timeout 10 ./target.out < ./testcase/in/in$i 1> ./output/out$i 2> /dev/null); res=$?; echo $res > ./res) 2>&1 ) | grep real | sed 's/real.*\t.*m//g' | sed 's/s//')
    echo Time : "$nowtime"sec
    res=$(cat ./res)
    if [ "$res" != "0" -a "$res" != "124" ]; then
        echo -e -n "Case $i \033[0;37m\033[5;41m Runtime Error \033[0;39m"
    else
        runtime=$(echo "scale=5; $runtime + $nowtime" | ./bc)
        origin=$(head -n 1 ./testcase/out/out$i)
        users=$(head -n 1 ./output/out$i)
        if [ "$origin" != "$users" ]; then
            echo -e "Case $i \033[0;37m\033[1;41m Wrong Answer \033[0;39m in phase Yes No determination"
            continue
        fi
        if [ $origin = "YES" ]; then
            count_yes=$(echo $count_yes + 1 | ./bc)
        fi
        echo "Ans  ": $origin
        echo $(cat ./testcase/in/in$i ./output/out$i) > ./tmp
        ./check.out < ./tmp > ./tmp2
        res=$?
        cat ./tmp2
        nowcost=$(tail -n 1 ./tmp2 | sed 's/K\ \ \ \ :\ //')
        if [ "$nowcost" = "" ]; then
            nowcost=0
        fi
        if [ $nowcost -ne 0 -a $i -le 100 ]; then
            list_cost=("${list_cost[@]}" $nowcost)
            if [ $min_cost -gt $nowcost ]; then
                min_cost=$nowcost
            fi
            if [ $max_cost -lt $nowcost ]; then
                max_cost=$nowcost
            fi
            if [ $(echo "$min_time > $nowtime" | ./bc) -eq 1 ]; then
                min_time=$nowtime
            fi
            if [ $(echo "$max_time < $nowtime" | ./bc) -eq 1 ]; then
                max_time=$nowtime
            fi
        fi
        totalcost=$(echo $totalcost + $nowcost | ./bc )
        if [ $res -eq 0 ]; then
            echo -e "Case $i \033[0;37m\033[0;44m Accepted \033[0;39m "
            count_ac=$(echo $count_ac + 1 | ./bc )
        else
            echo -e "Case $i \033[0;37m\033[1;41m Wrong Answer \033[0;39m "
        fi
    fi
done

sum=0
averagecost=$(echo $totalcost / $count_yes | ./bc)
for i in ${list_cost[@]}
do
    tmp=$(echo "$i - $averagecost" | ./bc)
    sum=$(echo "$sum + ( $tmp * $tmp )" | ./bc)
done
sum=$(echo $sum / "${#list_cost[@]}" | ./bc)

echo
echo "---------------"
echo
echo "[Result]"
echo "AC Count              : $count_ac / $CASE_N"
echo "Total Time(WithoutRE) : $runtime"sec
echo "Average Time          : $(echo 'scale=5;' $runtime / $CASE_N | ./bc)"sec
echo "Sum of K              : $totalcost"
echo "Average K             : $averagecost"
echo "Max K                 : $max_cost"
echo "Min K                 : $min_cost"
echo "Bunsan of K           : $sum"
echo "Hyoujunhensa of K     : $(echo "sqrt( $sum )" | ./bc)"
echo "Max Time(if ans==YES) : $max_time"
echo "Min Time(if ans==YES) : $min_time"
rm -f ./tmp ./tmp2 ./res ./target.out
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_TMP

