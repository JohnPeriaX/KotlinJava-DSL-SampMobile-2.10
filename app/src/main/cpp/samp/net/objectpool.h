#pragma once

#define INVALID_OBJECT_ID	0xFFFF

class CObjectPool
{
public:
	CObjectPool();
	~CObjectPool();

	bool New(OBJECTID ObjectID, int iModel, CVector vecPos, CVector vecRot, float fDrawDistance);
	bool Delete(OBJECTID ObjectID);

	void Process();

	// 0.3.7
	CObject* GetAt(OBJECTID ObjectID) {
		if (ObjectID <= MAX_OBJECTS && m_bObjectSlotState[ObjectID])
			return m_pObjects[ObjectID];
		
		return nullptr;
	}

    CObject *GetObjectFromGtaPtr(CEntityGTA *pGtaObject)
    {
        uint16_t x=1;

        while(x!=MAX_OBJECTS)
        {
            if(m_pObjects[x])
                if(pGtaObject == m_pObjects[x]->m_pEntity) return m_pObjects[x];

            x++;
        }

        return 0;
    }

	OBJECTID FindIDFromGtaPtr(CPhysical* pGtaObject);

	CObject* FindObjectFromGtaPtr(CPhysical* pGtaObject);

	void ProcessMaterialText();

private:
	int			m_iObjectCount;
	bool		m_bObjectSlotState[MAX_OBJECTS];
	CObject*	m_pObjects[MAX_OBJECTS];
};