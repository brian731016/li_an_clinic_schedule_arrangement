# Shift Scheduling

## 現在我將這個程式結合google apps script使它可以在google sheet中以較美觀的方式呈現

[The link of my google spreadsheet](https://docs.google.com/spreadsheets/d/1mWSDuDxTBBhjWv3Jr0bc-mfEbtZBjhNBNxnHiXmh7p4/edit?usp=sharing)

## Detail Feature Description

一個正職一個月要做43或44節，根據當月是小月或大月決定。

可以修改一天有幾節、一節要幾個人，但默認是所有不同工作的節次長一樣，所以如果不一樣的話要另外修改code。

員工可以在這個月排班前提前説要請假的節次。

此程式會盡全力排出合理的班表（優先保證合理性。例如，一節需要兩個員工的時候不會把一個人重複排兩次在那節、一節同時需要護士、行政時不會將一個人同時排做護士、行政⋯⋯）。在保證合理性的前提下，程式可能會有少量節次無法排入，這方面需要手動調整。

程式會優先排正直的班表，並且優先讓一個人在一天內連續上兩節，並且排班有一定的隨機性，會打散班表。

## Usage

1. 決定有幾種不同工作
2. 對於不同工作：
   1. 決定正職、兼職人數
   2. 給所有正職員工一個ID
   3. 給所有兼職員工一個ID
   4. 決定節次的結構（詳細表示方法在下方例子演示）
   5. 決定所有兼職員工要上的節的數量（輸入按照輸入ID的順序）
   6. 決定總共（將所有員工總）有多少節要請假
   7. 輸入所有要請假的節次（員工ID 幾號 第幾節）
   
3. 輸入年份
4. 輸入月份
5. 輸入這個月的1號是星期幾（星期天的話輸入0）

## Usage Example

The input of this example is in `example_input.txt` and `example_output.txt` is one of the output of the example.

1. 有兩種不同工作（護士、行政）

2. 對於護士：
    1. 兩個正職、兩個兼職
    2. ID 分別為 1 2
    3. ID 分別為 3 4（其中4還有當行政的兼職）
    4. 對於星期一到五、星期六：
        - 星期一到五一天3節，分別要2, 2, 1個護士
        - 星期六一天2節，分別要2,2個護士
        - `3[sessions] 2 2 1`
        - `2[sessions] 2 2`
    4. 這個例子中給兼職3 4分別上20 14節
    4. 這個例子中總共有11筆請假：
    7. - 1想要請：1號的0, 1節、28號的0, 1節
       - 2想要請：7號的1, 2節、8號的0, 1節
       - 3不用請假
       - 4想要請：11號的0, 1, 2節
    
    對於行政：
    
    ⋯⋯
    
    注意到4同時有做護士、行政，所以兩個工作都有出現ID 4。
    
2. 年份2025
    
2. 月份11月
    
5. 11/1是星期六

## input format

```
type_of_jobs_c

for type_of_jobs_c times{
    full_time_c part_time_c
    [full_time_id]
    [part_time_id]
    session_struct_c [worker_c of each session] (Mon-Fri)
    session_struct_c [worker_c of each session] (Sat)
    [part_time_session_c] (in the same order of inputing part_time_id)
    off_sessions_count
    for off_sessions_count times{
        id off_session_day_in_month session_index
    }
}

year
month
first_day_of_the_month_in_week
```

## Short Note

我在寫完所有程式之後才發現，排班的英文應該是 schedule shift。晚班的英文應該是night shift，但我在程式裡面都用session來稱呼節，schedules來稱呼班表，arrange schedule來稱呼排班。
