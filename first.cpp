#include <algorithm>
#include<bits/stdc++.h>
#include <ios>
using namespace std;
using vvi=vector<vector<int>>;
// <some>_c means the number of <some> (stands for some_count)

void fisher_yates_suffle(vector<int>&arr){
    random_device rd;
    mt19937 gen(rd()); // 隨機數生成器
    for (int i = arr.size() - 1; i > 0; --i) {
        uniform_int_distribution<> dis(0, i);
        int j = dis(gen);
        swap(arr[i], arr[j]);
    }
}

// 0-based, so the off-day date should be subtract by 1
class Arrange_schedule{
protected:
    int year;
    int month;
    int first_day_of_the_month;
    int day_c_of_this_month;
    int employee_c;
    int part_time_c;
    int session_per_employee;
    vector<set<int>> employee_off_day;
    vector<set<int>> part_time_off_day;
    vvi schedule;
    vector<int>session_per_day;
    Arrange_schedule(int y,int m,int f,int e,int p,vector<int>spd,vector<set<int>>eod,vector<set<int>>ptod)
        :year(y),month(m),first_day_of_the_month(f%7),employee_c(e),part_time_c(p),session_per_day(spd),employee_off_day(eod),part_time_off_day(ptod){
        // set the month day count according to year
        vector<int> month_to_day_c={-1,31,28,31,30,31,30,31,31,30,31,30,31};
        if (month==2 && (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            day_c_of_this_month=29;
        }else{
            day_c_of_this_month=month_to_day_c[month];
        }
        session_per_employee=(day_c_of_this_month==31?44:43);
    }

    vector<int> create_schedule_order(){
        vector<int>schedule_employee_order(session_per_employee*employee_c);
        for(int i=0;i<employee_c;i++){
            for(int j=0;j<session_per_employee;j++){
                schedule_employee_order[i*session_per_employee+j]=i;
            }
        }
        fisher_yates_suffle(schedule_employee_order);
        return schedule_employee_order;
    }

    vector<int> find_available_sessions(vector<int>one_day_schedule,int day){
        vector<int>available_sessions;
        for(int i=0;i<one_day_schedule.size();i++){
            if(day==6 && i==2){
                continue;
            }
            if(one_day_schedule[i]==-1){
                available_sessions.push_back(i);
            }
        }
        return available_sessions;
    }

    void the_next_day(int &cur_day,int &cur_date){
        cur_day=(cur_day+1)%7;
        cur_date=(cur_date+1)%day_c_of_this_month;
    }

    void Sat_arrange_employee_shedule(int &cur_date,int cur_scheduling_employee,vector<int>&session_pending_c,vector<int>available_session,int random_schedule_session){
        if(available_session.size()==1){
            schedule[cur_date][available_session[0]]=cur_scheduling_employee;
            session_pending_c[cur_scheduling_employee]--;
        }else{
            schedule[cur_date][0]=cur_scheduling_employee;
            schedule[cur_date][1]=cur_scheduling_employee;
            session_pending_c[cur_scheduling_employee]-=2;
        }
    }

    void Mon_to_Fri_arrange_employee_shedule(int &cur_date,int cur_scheduling_employee,vector<int>&session_pending_c,vector<int>available_session,int random_schedule_session){
        if(available_session.size()<2){
            schedule[cur_date][available_session[0]]=cur_scheduling_employee;
            session_pending_c[cur_scheduling_employee]--;
        }else if(available_session.size()==2){
            if(abs(available_session[0]-available_session[1])==1){
                schedule[cur_date][available_session[0]]=cur_scheduling_employee;
                schedule[cur_date][available_session[1]]=cur_scheduling_employee;
                session_pending_c[cur_scheduling_employee]-=2;
            }else{
                schedule[cur_date][available_session[random_schedule_session]]=cur_scheduling_employee;
                session_pending_c[cur_scheduling_employee]--;
            }
        }else{  // available_session.size==3
            schedule[cur_date][random_schedule_session]=cur_scheduling_employee;
            schedule[cur_date][random_schedule_session+1]=cur_scheduling_employee;
            session_pending_c[cur_scheduling_employee]-=2;
        }
    }

    void arrange_one_time(int &cur_day,int &cur_date,int cur_scheduling_employee,vector<int>&session_pending_c){
        if(cur_day==0){
            return;
        }

        if(session_pending_c[cur_scheduling_employee]==1){
            //todo
        }

        random_device rd;
        mt19937 gen(rd()); // 隨機數生成器
        uniform_int_distribution<> dis(0, 1);
        int random_schedule_session=dis(gen);

        vector<int> available_session=find_available_sessions(schedule[cur_date],cur_day);
        while(available_session.empty()){
            the_next_day(cur_day,cur_date);
            available_session=find_available_sessions(schedule[cur_date],cur_day);
        }
        int init_date=cur_date;
        while(available_session.size()<2){
            the_next_day(cur_day,cur_date);
            available_session=find_available_sessions(schedule[cur_date],cur_day);
            if(cur_date==init_date){
                break;
            }
        }

        if(cur_day!=6){
            Mon_to_Fri_arrange_employee_shedule(cur_date,cur_scheduling_employee,session_pending_c, available_session,random_schedule_session);
            return;
        }

        if(cur_day==6){
            Sat_arrange_employee_shedule(cur_date,cur_scheduling_employee,session_pending_c, available_session,random_schedule_session);
        }

    }

    // the order is to create the feeling of random arrangement
    void arrange_employee(){
        vector<int>schedule_employee_order=create_schedule_order();
        int left_c=employee_c;
        vector<int>session_pending_c(employee_c,session_per_employee);
        vector<bool>done_scheduling(employee_c,false);
        int cur_day=first_day_of_the_month;
        int cur_date;
        for(int i=0;i<schedule_employee_order.size() && left_c>0;i++){
            int cur_scheduling_employee=schedule_employee_order[i];
            if(session_pending_c[cur_scheduling_employee]==0){
                if(!done_scheduling[cur_scheduling_employee]){
                    done_scheduling[cur_scheduling_employee]=true;
                    left_c--;
                }
                continue;
            }
            if(!employee_off_day[cur_scheduling_employee].count(cur_date)){
                continue;
            }
            arrange_one_time(cur_day,cur_date,cur_scheduling_employee,session_pending_c);
            the_next_day(cur_day, cur_date);
        }
    }

public:
    static void handle_input(); // 0-based, so the off-day date should be subtract by 1
    void arrange(){
        schedule=vvi(day_c_of_this_month,vector<int>(*max_element(session_per_day.begin(),session_per_day.end()),-1));
        arrange_employee();
        // arrange_part_time();
        
    }
    void print_one_day_schedule(int date,int day,vector<int>one_day_schedule){
        vector<string>day_num_to_day_name = {
            "Sunday",    // day 0
            "Monday",    // day 1
            "Tuesday",   // day 2
            "Wednesday", // day 3
            "Thursday",  // day 4
            "Friday",    // day 5
            "Saturday"  // day 6
        };
        cout<<year<<'/'<<month<<'/'<<date<<' '<<day_num_to_day_name[day]<<' ';
        for(int i=0;i<one_day_schedule.size();i++){
            cout<<"session "<<i+1<<": "<<one_day_schedule[i]<<' ';
        }
    }
    void print_schedule(){
        for(int i=0;i<schedule.size();i++){
            print_one_day_schedule(i+1,(first_day_of_the_month+i)%7,schedule[i]);
        }
    }
};

// class Arrange_nurse_schedule:public Arrange_schedule{
// };
//
// class Arrange_admin_schedule{
// };

int main(int argc,char* argv[]){


    int year, month, first_day_of_the_month;
    cin>>year>>month>>first_day_of_the_month;
    cout<<"testing over\n";
}
