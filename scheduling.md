2 nurse fixed and 2 part-time
1 admin fixed and 2 part-time
both 44 sessions morning afternoon evening

can reserve off-session

Mon-Fri
morning 2n 1a
afternoon 1n 1n
evenning 2n 1a

Sat
morning 2n 1a
afternoon 2n 1n

Fen is both n and a, so she can't be n and a at the same session.



## featrues

- can handle multiple workers in one sessino
- can handle the conflict that one worker can be both nurse and admin and will not let zer get two jobs at the same session

## input needed


## input format

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

### An example

2

2 2 
1 2 
3 4 
3 2 1 2
2 2 2
20 14
11
1 1 0
1 1 1 
1 28 0
1 28 1 
2 7 1 
2 7 2 
2 8 0
2 8 1 
4 11 0 
4 11 1 
4 11 2

1 2 
5 
6 4 
3 1 1 1
2 1 1
17 10
9
6 27 0
6 27 1
6 27 2
6 28 0
6 28 1
6 28 2
4 11 0
4 11 1 
4 11 2

2025
11
6 

