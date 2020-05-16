/*time out*/
#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

struct employee{
	int parent_id;
	int current_id;
	int happy_v;
	vector<employee*> son_list;

	employee(int pid, int cid, int happy)
	{
		parent_id = pid;
		current_id = cid;
		happy_v = happy;
	};
};

class company {
public:
	company(int bossid, int membernum, bool isdebug, vector<int> happlist);
	void addemployee(int parent_id, int current_id);
	void freecompany(employee *root);
	void printcompany(employee *root);
	void calcHappyValue();
	int getmaxhappy();
	void print_c();
	void free_c();
public:
	employee* m_company;
	vector<int> m_happylist;
	vector<int> m_dp_yes;
	vector<int> m_dp_no;
	vector<employee*> m_root;
	int m_boss;
	int m_member;
	bool m_print;
};

company::company(int bossid, int membernum, bool isdebug, vector<int> happylist)
{
	m_boss = bossid;
	m_member = membernum;
	m_happylist = happylist;
	m_print = isdebug;
	m_company = new employee(m_boss, m_boss, m_happylist[m_boss-1]);
	m_dp_yes.assign(m_member, 0);
	m_dp_no.assign(m_member, 0); 
	m_root.assign(m_member, 0);
	m_root[m_company->current_id-1] = m_company;
}

void company::addemployee(int parent_id, int current_id)
{
	if (m_root[parent_id-1] != NULL)
	{
		employee* newemployee = new employee(parent_id, current_id, m_happylist[current_id - 1]);
		m_root[parent_id-1]->son_list.push_back(newemployee);
	}
	else
	{
		queue<employee*> bfs_vist;
		bfs_vist.push(m_company);
		while (!bfs_vist.empty())
		{
			employee* curNode = bfs_vist.front();
			bfs_vist.pop();

			if (curNode->current_id == parent_id)
			{
				employee* newemployee = new employee(parent_id, current_id, m_happylist[current_id - 1]);
				curNode->son_list.push_back(newemployee);
				//record;
				m_root[newemployee->current_id-1] = newemployee;
				return;
			}

			for (int i = 0; i < curNode->son_list.size(); ++i)
			{
				bfs_vist.push(curNode->son_list[i]);
			}
		}
	}
}

void company::freecompany(employee *root)
{
	if (m_print)
	{
		if (root->son_list.size() == 0)
		{
			root->son_list.clear();
			delete root;
		}
		else
		{
			for (int i = 0; i < root->son_list.size(); ++i)
			{
				freecompany(root->son_list[i]);
			}
			root->son_list.clear();
			delete root;
		}
	}
}

void company::printcompany(employee *root)
{
	if (m_print)
	{
		if (root->son_list.size() == 0)
		{
			cout << root->parent_id << " ";
			cout << root->current_id << " ";
			cout << root->happy_v << " ";
			cout << root->son_list.size() << endl;
		}
		else
		{
			for (int i = 0; i < root->son_list.size(); ++i)
			{
				printcompany(root->son_list[i]);
			}
		}
	}
}

void company::calcHappyValue()
{
	queue<employee*> bfs_vist;
	stack<employee*> dfs_vist;

	bfs_vist.push(m_company);
	dfs_vist.push(m_company);

	while (!bfs_vist.empty())
	{
		employee* curNode = bfs_vist.front();
		bfs_vist.pop();
		
		for (int i = 0; i < curNode->son_list.size(); ++i)
		{
			bfs_vist.push(curNode->son_list[i]);
			dfs_vist.push(curNode->son_list[i]);
		}
	}

	//dfs from son to parent
	while (!dfs_vist.empty())
	{
		employee* curNode = dfs_vist.top();
		dfs_vist.pop();

		if (curNode->son_list.size() == 0)
		{
			m_dp_yes[curNode->current_id-1] = curNode->happy_v;
			m_dp_no[curNode->current_id-1] = 0;
		}
		else
		{
			for (int i = 0; i < curNode->son_list.size(); ++i)
			{
				m_dp_no[curNode->current_id-1] += max(m_dp_yes[curNode->son_list[i]->current_id-1], m_dp_no[curNode->son_list[i]->current_id-1]);
				m_dp_yes[curNode->current_id-1] += m_dp_no[curNode->son_list[i]->current_id-1];
			}
			m_dp_yes[curNode->current_id - 1] += curNode->happy_v;
		}
		if (m_print)
		{
			cout << curNode->parent_id << "-" << curNode->current_id << " " << m_dp_yes[curNode->current_id - 1] << " " << m_dp_no[curNode->current_id - 1] << endl;
		}
	}
}

int company::getmaxhappy()
{
	return max(m_dp_yes[m_boss - 1], m_dp_no[m_boss - 1]);
}

void company::print_c()
{
	printcompany(m_company);
}

void company::free_c()
{
	freecompany(m_company);
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
		company mycompany(boss, employeenum, false, happylist);

		int parentid, currentid;
		for (int i = 0; i < employeenum - 1; i++)
		{
			cin >> parentid >> currentid;
			mycompany.addemployee(parentid, currentid);
		}
		mycompany.print_c();
		mycompany.calcHappyValue();
		cout << mycompany.getmaxhappy() << endl;
		mycompany.free_c();
	}
	return 0;
}
