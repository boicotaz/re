#! /bin/bash

cat res.txt | grep prod | cut -d ":" -f2 > producer.txt
cat res.txt | grep cons | cut -d ":" -f2 > consumer.txt
# cat res.txt | grep a:drift | cut -d ":" -f3 > second_timer_drift.txt
# cat res.txt | grep b:drift | cut -d ":" -f3 > decisecond_timer_drift.txt
cat res.txt | grep c:drift | cut -d ":" -f3 > centisecond_timer_drift.txt