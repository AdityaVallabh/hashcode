#include <bits/stdc++.h>
#define ll long long
#define pb push_back
#define ff first
#define ss second
#define Flash ios_base::sync_with_stdio(false); cin.tie(NULL)
#define all(v) v.begin(),v.end()
#define rev(v) reverse(all(v))
#define sot(v) sort(all(v))
#define printa(a,L,R) for(int i=L;i<R;i++) cout<<a[i]<<(i==R-1?"\n":" ")
#define printv(a) printa(a,0,a.size())
#define print2d(a,end,c) for(int i=0;i<end;i++) for(int j=0;j<c;j++) cout<<a[i][j]<<(j==c-1?"\n":" ")

const int N = (int) 1e5 + 7;
const int MOD = (int) 1e9 + 7;
const ll INF = (ll) 1e18;

const int THRESHOLD = 0;

using namespace std;

struct Coder {
    string name;
    map<string, int> skills;
    int availableAt;

    Coder(string _name, map<string,int> _skills) {
        name = _name;
        skills = _skills;
        availableAt = 0;
    }
};

struct Project {
    string name;
    int duration;
    int score;
    int deadline;
    vector<pair<string,int>> roles;

    Project(string _name, int _daysToComplete, int _score, int _bestBeforeDay, vector<pair<string,int>> skills) {
        name = _name;
        duration = _daysToComplete;
        score = _score;
        deadline = _bestBeforeDay;
        roles = skills;
    }
};

map<string,Coder*> codersMap;
map<string,vector<Coder*>> skillsMap;
vector<Project*> projects;

void read_input() {
	int c, p;
	cin >> c >> p;

	while (c--) {
		string name;
		int n;
		cin >> name >> n;
		map<string, int> skills;
		while (n--) {
			string skill;
			int level;
			cin >> skill >> level;
			skills[skill] = level;
		}
		auto coder = new Coder(name, skills);
        codersMap[coder->name] = coder;
        for(auto skill: coder->skills) {
            skillsMap[skill.first].push_back(coder);
        }
	}

	 while (p--) {
	 	string name;
		int daysToComplete, score, bestBeforeDay, r; 
		cin >> name >> daysToComplete >> score >> bestBeforeDay >> r;
		vector<pair<string, int>> skills;

		while (r--) {
			string skill;
			int level;
			cin >> skill >> level;
			skills.push_back({skill, level});
            skillsMap[skill];
		}
		projects.push_back(new Project(name, daysToComplete, score, bestBeforeDay, skills));
	 }
	 return;
}

void sortSkill(string skillName) {
    sort(all(skillsMap[skillName]), [&](Coder *a, Coder *b) {
        if (a->skills[skillName] != b->skills[skillName]) {
            return a->skills[skillName] < b->skills[skillName];
        }
        return a->availableAt < b->availableAt;
    });
}

void sortSkillsMap() {
    // for(auto coder: codersMap) {
    //     for(auto skill: skillsMap) {
    //         if(coder.second->skills.find(skill.first) == coder.second->skills.end()) {
    //             skillsMap[skill.first].push_back(coder.second);
    //         }
    //     }
    // }
    for(auto skill: skillsMap) {
        sortSkill(skill.first);
    }
}

vector<Coder*>::iterator findCoder(
    Project *project, vector<Coder*> &available, int req, string skillName, unordered_set<string> &taken) {
    auto coder = lower_bound(all(available), req, [&](Coder *a, int target) {
        return a->skills[skillName] < target;
    });
    while(coder != available.end() && (taken.find((*coder)->name) != taken.end() || (*coder)->availableAt - THRESHOLD > project->deadline-project->duration)) {
        coder++;
    }
    return coder;
}

vector<pair<Project*,vector<Coder*>>> process() {
    int currentDay = 0;
    vector<pair<Project*,vector<Coder*>>> res;
    sort(all(projects), [](Project *a, Project *b) {
        if(a->deadline != b->deadline) {
            return a->deadline < b->deadline;
        }
        if(a->score != b->score) {
            return a->score > b->score;
        }
        if (a->duration != b->duration) {
            return a->duration < b->duration;
        }
        if(a->roles.size() != b->roles.size()) {
            return a->roles.size() < b->roles.size();
        }
        return a->name < b->name;
    });
    for(auto project: projects) {
        // if(currentDay + project->duration > project->deadline) continue;
        bool allRolesDone = true;
        vector<Coder*> coders;
        unordered_set<string> taken;
        for(auto role: project->roles) {
            string skillName = role.first;
            int req = role.second;
            vector<Coder*> available = skillsMap[skillName];
            auto coder = findCoder(project, available, req-1, skillName, taken);
            if(coder == available.end()) {
                allRolesDone = false;
                break;
            }
            if((*coder)->skills[skillName] == req-1) {
                bool mentee = false;
                for(auto mentor: taken) {
                    if(codersMap[mentor]->skills[skillName] >= req) {
                        mentee = true;
                        break;
                    }
                }
                if(!mentee) {
                    coder = findCoder(project, available, req, skillName, taken);
                    if(coder == available.end()) {
                        allRolesDone = false;
                        break;
                    }
                }
            }
            taken.insert((*coder)->name);
            coders.push_back(*coder);
        }
        if(allRolesDone) {
            currentDay += project->duration;
            int projectStart = 0;
            for(int i = 0; i < project->roles.size(); i++) {
                projectStart = max(projectStart, coders[i]->availableAt);
                string skillName = project->roles[i].first;
                int reqLevel = project->roles[i].second;
                if(coders[i]->skills[skillName] <= reqLevel) {
                    coders[i]->skills[skillName]++;
                    sortSkill(skillName);
                }
            }
            for(auto coder: coders) {
                coder->availableAt = projectStart + project->duration;
            }
            res.push_back({project, coders});
        }
    }
    return res;
}

void print_result(vector<pair<Project*,vector<Coder*>>> res) {
    cout << res.size() << '\n';
    for(auto p: res) {
        cout << p.first->name << '\n';
        for(auto coder: p.second) {
            cout << coder->name << ' ';
        }
        cout << '\n';
    }
}

int main() {
    Flash;
    read_input();
    sortSkillsMap();
    auto res = process();
    print_result(res);
    return 0;
}
