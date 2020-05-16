#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class company {
public:
	company(int member, int boss, vector<int> happylist);
	void addemployee(int parent_id, int current_id);
	void calchappy(vector<int> root, int boss);
	int getmaxhappy();
public:
	vector<int> m_happylist;
	vector<int> m_dp_yes;
	vector<int> m_dp_no;
	int m_boss;
	vector<vector<int> > m_root;
};

company::company(int member, int boss, vector<int> happylist)
{
	m_happylist = happylist;
	m_dp_yes.assign(member, 0);
	m_dp_no.assign(member, 0);
	m_boss = boss;
	for (int i = 0; i < member; i++)
	{
		m_root.push_back(vector<int>());
	}
}

void company::addemployee(int parent_id, int current_id)
{
	m_root[parent_id-1].push_back(current_id-1);
}

void company::calchappy(vector<int> root, int boss)
{
	m_dp_yes[boss] = m_happylist[boss];
	m_dp_no[boss] = 0;

	for (int i = 0; i < root.size(); ++i)
	{
		int curid = root[i];
		calchappy(m_root[curid], curid);
		m_dp_no[boss] += max(m_dp_no[curid], m_dp_yes[curid]);
		m_dp_yes[boss] += m_dp_no[curid];
	}
}

int company::getmaxhappy()
{
	calchappy(m_root[m_boss-1], m_boss-1);
	return max(m_dp_yes[m_boss - 1], m_dp_no[m_boss - 1]);
}

int main()
{
	int employeenum, boss;

	while (cin >> employeenum >> boss)
	{
		int tmp_val;
		vector<int> happylist;
		for (int i = 0; i < employeenum; i++)
		{
			cin >> tmp_val;
			happylist.push_back(tmp_val);
		}
		//create
		company mycompany(employeenum, boss, happylist);

		int parentid, currentid;
		for (int i = 0; i < employeenum - 1; i++)
		{
			cin >> parentid >> currentid;
			mycompany.addemployee(parentid, currentid);
		}

		cout << mycompany.getmaxhappy() << endl;
	}
	return 0;
}
