#include "Subject.h"

using namespace std;

namespace Core
{
	Subject::Subject()
	{
	}

	void Subject::Attach(Obeserver * pObserver)
	{
		m_observerList.push_back(pObserver);
	}

	void Subject::Detach(Obeserver * pObserver)
	{
		for (vector<Obeserver *>::iterator iter = m_observerList.begin();
			iter != m_observerList.end();
			++iter)
		{
			if (*iter == pObserver)
			{
				m_observerList.erase(iter);
				break;
			}
		}
	}

	void Subject::NotifyAhead()
	{
		for (vector<Obeserver *>::iterator iter = m_observerList.begin();
			iter != m_observerList.end();
			++iter)
		{
			(*iter)->UpdateAhead();
		}
	}

	void Subject::NotifyBehind()
	{
		for (vector<Obeserver *>::iterator iter = m_observerList.begin();
			iter != m_observerList.end();
			++iter)
		{
			(*iter)->UpdateBehind();
		}
	}

	Subject::~Subject()
	{
	}
}