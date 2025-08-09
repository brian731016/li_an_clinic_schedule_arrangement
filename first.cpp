#include<bits/stdc++.h>
using namespace std;
using vvi=vector<vector<int>>;
using pii=pair<int,int>;
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
class Position_info{
public:
    int full_time_c;
    int part_time_c;
    vector<int> full_time_id;
    vector<int> part_time_id;
    // map<int,int> full_time_id_to_index;
    // map<int,int> part_time_id_to_index;
    vvi session_struct; // the j-th session of day-i has session_struct[i][j] workers
    vector<int> part_time_session_c;
    map<int,set<pii>> off_sessions;   // {key,element}={id,{day_of_month,session_idx}}
    map<int,int> full_time_sessions_pending_c;  // initialize in Arrange_schedule
    map<int,int> part_time_sessions_pending_c;  // initialize in Arrange_schedule
    void input_and_init(){
        cin>>full_time_c;
        cin>>part_time_c;
        for(int i=0,id;i<full_time_c;i++){
            cin>>id;
            full_time_id.push_back(id);
        }
        for(int i=0,id;i<part_time_c;i++){
            cin>>id;
            part_time_id.push_back(id);
        }

        /* session_struct input and init*/
        session_struct=vvi(7,vector<int>());
        int session_struct_c;
        cin>>session_struct_c;
        for(int i=0,worker_c;i<session_struct_c;i++){
            cin>>worker_c;
            session_struct[1].push_back(worker_c);
        }
        for(int i=2;i<=5;i++){
            session_struct[i]=session_struct[1];
        }
        for(int i=0,worker_c;i<session_struct_c;i++){
            cin>>worker_c;
            session_struct[6].push_back(worker_c);
        }
        for(int i=0,session_count;i<part_time_c;i++){
            cin>>session_count;
            part_time_session_c.push_back(session_count);
        }
        /* end */

        int off_sessions_count;
        cin>>off_sessions_count;
        for(int id,off_session_day_in_month,session_index;off_sessions_count--;){
            cin>>id>>off_session_day_in_month>>session_index;
            if(!off_sessions.count(id)){
                off_sessions[id]=set<pii>();
            }
            off_sessions[id].insert(pii(off_session_day_in_month,session_index));
        }
    }
};

class Date{
private:
    int day_c_of_this_month;
public:
    int in_month;   // day of month
    int in_week;    // day of week
    Date(int dc,int m,int w):day_c_of_this_month(dc),in_month(m),in_week(w){}
    void go_to_the_next_day(){
        in_month=(in_month+1)%day_c_of_this_month;
        in_week=(in_week+1)%7;
    }
    bool operator==(const Date other) const {
        return day_c_of_this_month==other.day_c_of_this_month
            && in_month==other.in_month
            && in_week==other.in_week;
    }
};

class Arrange_schedule{
private:
    int year;
    int month;
    int first_day_of_the_month_in_week;
    int day_c_of_this_month;
    int session_per_full_time;
    int type_of_jobs_c;
    vector<vector<vector<set<int>>>> schedule;
    vector<Position_info> position_infos;

    vector<int> create_schedule_order(int type_of_job,int is_full_time){
        Position_info cur_pos_info=position_infos[type_of_job];
        if(is_full_time){
            vector<int>schedule_worker_order(session_per_full_time*cur_pos_info.full_time_c);
            for(int i=0;i<cur_pos_info.full_time_c;i++){
                for(int j=0;j<session_per_full_time;j++){
                    schedule_worker_order[i*session_per_full_time+j]=i;
                }
            }
            fisher_yates_suffle(schedule_worker_order);
            return schedule_worker_order;
        }
        // is part-time
        vector<int>schedule_worker_order;
        for(int i=0;i<cur_pos_info.part_time_c;i++){
            for(int j=0;j<cur_pos_info.part_time_session_c[i];j++){
                schedule_worker_order.push_back(i);
            }
        }
        fisher_yates_suffle(schedule_worker_order);
        return schedule_worker_order;
    }

    bool has_conflict(Date date,int id){  // the worder <id> has another jobs in other job type
        for(auto one_job_schedule : schedule){
            for(auto one_day_schedul : one_job_schedule){
                for(auto one_session : one_day_schedul){
                    if(one_session.count(id)){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    vector<int> find_available_sessions(int type_of_job,Date date,int id){
        Position_info cur_pos_info=position_infos[type_of_job];
        vector<int>available_sessions;
        for(int session_idx=0;session_idx<cur_pos_info.session_struct[date.in_week].size();session_idx++){
            if(cur_pos_info.off_sessions[id].count(pii(date.in_month,session_idx))
                    || schedule[type_of_job][date.in_month][session_idx].size()==cur_pos_info.session_struct[date.in_week][session_idx]
                    || schedule[type_of_job][date.in_month][session_idx].count(id)
                    || has_conflict(date,id)){
                continue;
            }
            available_sessions.push_back(session_idx);
        }
        return available_sessions;
    }

    pair<int,vector<int>> find_available_day_sessions(int type_of_job,int cur_id,Date &cur_day){
        vector<int> available_session=find_available_sessions(type_of_job,cur_day,cur_id);
        Date init_day=cur_day;
        while(available_session.empty()){
            cur_day.go_to_the_next_day();
            available_session=find_available_sessions(type_of_job,cur_day,cur_id);
            if(cur_day==init_day){
                return {0,available_session};
            }
        }
        init_day=cur_day;
        while(available_session.size()<2){
            cur_day.go_to_the_next_day();
            available_session=find_available_sessions(type_of_job,cur_day,cur_id);
            if(cur_day==init_day){
                return {1,available_session};
            }
        }
        init_day=cur_day;
        while(available_session.size()<3
                || !(available_session.size()==2 && abs(available_session[0]-available_session[1])==1)
                ){
            cur_day.go_to_the_next_day();
            available_session=find_available_sessions(type_of_job,cur_day,cur_id);
            if(cur_day==init_day){
                return {2,available_session};
            }
        }
        if(available_session.size()==2){
            return {3,available_session};
        }
        return {4,available_session};
    }

    void arrange_full_time_one_time(int type_of_job,Date &cur_day,int cur_id){
        random_device rd;
        mt19937 gen(rd()); // 隨機數生成器
        uniform_int_distribution<> dis(0, 1);
        int random_num=dis(gen);
        if(position_infos[type_of_job].full_time_sessions_pending_c[cur_id]==1){
            auto [available_type,available_session]=find_available_day_sessions(type_of_job,cur_id,cur_day);
            if(available_type==1){
                schedule[type_of_job][cur_day.in_month][available_session[0]].insert(cur_id);
                position_infos[type_of_job].full_time_sessions_pending_c[cur_id]--;
            }else if(available_type==2){
                schedule[type_of_job][cur_day.in_month][available_session[random_num]].insert(cur_id);
                position_infos[type_of_job].full_time_sessions_pending_c[cur_id]--;
            }else if(available_type==3){
                schedule[type_of_job][cur_day.in_month][available_session[random_num]].insert(cur_id);
                position_infos[type_of_job].full_time_sessions_pending_c[cur_id]--;
            }else if(available_type==4){
                uniform_int_distribution<> dis_2(0, 2);
                random_num=dis_2(gen);
                schedule[type_of_job][cur_day.in_month][available_session[random_num]].insert(cur_id);
                position_infos[type_of_job].full_time_sessions_pending_c[cur_id]--;
            }
            return;
        }

        auto [available_type,available_session]=find_available_day_sessions(type_of_job,cur_id,cur_day);
        // available_type might be 0,1,2,3,4
        // {0,1,2,3,4} means {have no available session,
        //                      only have one session,
        //                      only have two non-consecutive sessions,
        //                      only have two consecutive sessions,
        //                      have tree consecutive sessions}
        if(available_type==1){
            schedule[type_of_job][cur_day.in_month][available_session[0]].insert(cur_id);
            position_infos[type_of_job].full_time_sessions_pending_c[cur_id]--;
        }else if(available_type==2){
            schedule[type_of_job][cur_day.in_month][available_session[random_num]].insert(cur_id);
            position_infos[type_of_job].full_time_sessions_pending_c[cur_id]--;
        }else if(available_type==3){
            schedule[type_of_job][cur_day.in_month][available_session[0]].insert(cur_id);
            schedule[type_of_job][cur_day.in_month][available_session[1]].insert(cur_id);
            position_infos[type_of_job].full_time_sessions_pending_c[cur_id]-=2;
        }else if(available_type==4){
            schedule[type_of_job][cur_day.in_month][available_session[random_num]].insert(cur_id);
            schedule[type_of_job][cur_day.in_month][available_session[random_num+1]].insert(cur_id);
            position_infos[type_of_job].full_time_sessions_pending_c[cur_id]-=2;
        }
    }

    void arrange_full_time_schedule(int type_of_job){
        Position_info cur_pos_info=position_infos[type_of_job];
        vector<int>schedule_full_time_order=create_schedule_order(type_of_job,true);
        int left_c=cur_pos_info.full_time_c;
        vector<bool>done_scheduling(cur_pos_info.full_time_c,false);
        Date cur_day(day_c_of_this_month,0,first_day_of_the_month_in_week);
        for(int cur_arranging_index:schedule_full_time_order){
            int cur_id=cur_pos_info.full_time_id[cur_arranging_index];
            if(cur_pos_info.full_time_sessions_pending_c[cur_id]==0){
                if(!done_scheduling[cur_arranging_index]){
                    done_scheduling[cur_arranging_index]=true;
                    left_c--;
                }
                continue;
            }
            arrange_full_time_one_time(type_of_job,cur_day,cur_id);
            cur_day.go_to_the_next_day();
        }
    }

    void print_one_session_schedule(int day_in_month,int day_in_week,int session_idx){
        for(int i=0;i<type_of_jobs_c;i++){
            cout<<"    job type "<<i+1<<": ";
            for(int id:schedule[i][day_in_month][session_idx]){
                cout<<id<<" ";
            }
            cout<<'\n';
        }
    }
    void print_one_day_schedule(int day_in_month,int day_in_week){
        vector<string>day_num_to_day_name = {
            "Sunday",    // day 0
            "Monday",    // day 1
            "Tuesday",   // day 2
            "Wednesday", // day 3
            "Thursday",  // day 4
            "Friday",    // day 5
            "Saturday"  // day 6
        };
        vector<int>session_c={0,3,3,3,3,3,2};
        cout<<year<<'/'<<month<<'/'<<day_in_month<<' '<<day_num_to_day_name[day_in_week]<<'\n';
        for(int i=0;i<session_c[day_in_week];i++){
            print_one_session_schedule(day_in_month,day_in_week,i);
        }
    }
public:
    void arrange_schedule(){
        for(int i=0;i<type_of_jobs_c;i++){
            arrange_full_time_schedule(i);
        }
    }

    void input_and_init(){
        cin>>type_of_jobs_c;
        position_infos=vector<Position_info>(type_of_jobs_c);
        for(Position_info &info:position_infos){
            info.input_and_init();
        }

        cin>>year>>month>>first_day_of_the_month_in_week;
        vector<int> month_to_day_c={-1,31,28,31,30,31,30,31,31,30,31,30,31};
        if (month==2 && (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            day_c_of_this_month=29;
        }else{
            day_c_of_this_month=month_to_day_c[month];
        }
        session_per_full_time=(day_c_of_this_month==31?44:43);

        // initialize sessions_pending_c for position_infos
        for(Position_info &info:position_infos){
            for(int i=0;i<info.full_time_c;i++){
                info.full_time_sessions_pending_c[info.full_time_id[i]]=session_per_full_time;
            }
            for(int i=0;i<info.part_time_c;i++){
                info.part_time_sessions_pending_c[info.part_time_id[i]]=info.part_time_session_c[i];
            }
        }
    }

    void print_schedule(){
        for(int i=0;i<day_c_of_this_month;i++){
            print_one_day_schedule(i+1,(first_day_of_the_month_in_week+i)%7);
            cout<<'\n';
        }
    }
};

int main(int argc,char* argv[]){
    Arrange_schedule li_an_arrange;
    li_an_arrange.input_and_init();
    li_an_arrange.arrange_schedule();
    li_an_arrange.print_schedule();
}
