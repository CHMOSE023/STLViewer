// Entity.h: interface for the CEntity class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "GeomCalc\cadbase.h"

class COpenGLDC;
class AFX_EXT_CLASS CEntity : public CObject  
{
protected:
	BOOL		m_bModified;
	CBox3D*		m_pBox;
	UINT		m_id;
	CString		m_name;
	COLORREF	m_color;
	BOOL		m_bVisible;
	BOOL		m_bHighlight;

public:
	enum Type {
		kSTLModel =0,
		kPart,
		kBezierCurve,
		kBezierSurface,
		kNurbsCurve,
		kNurbsSurface
	};

public:
	//constructor and destructor
	CEntity();
	virtual ~CEntity();

	//display
	virtual void Draw(COpenGLDC* pDC)=0;

	//serialize
	virtual	void Serialize(CArchive& ar);	

	//attrib accessing 
	BOOL		GetBox(CBox3D& box);	
	
	void		SetID(UINT nID);
	UINT		GetID();
	
	void		SetName(LPCTSTR name);
	CString		GetName();
	
	void		SetColor(COLORREF color);
	COLORREF	GetColor();

	void		SetVisible(BOOL bVisible);
	BOOL		IsVisible();

	void		SetHighlight(BOOL bHighlight);
	BOOL		IsHighlight();

	//get entity type
	virtual CEntity::Type type() const =0;

protected:
	virtual		void UpdateBox()=0;
};


//triangle chip
class AFX_EXT_CLASS CTriChip:public CObject
{

	DECLARE_SERIAL(CTriChip)
public:
	//attribs
	CPoint3D  vex[3];
	CVector3D normal;

public:
	//constructor && destructor
	CTriChip();
	CTriChip(const CPoint3D& v0,const CPoint3D& v1,const CPoint3D& v2,const CVector3D& norm);
	virtual ~CTriChip();

	//display
	virtual void Draw(COpenGLDC* pDC);

	//serialize
	virtual void Serialize(CArchive& ar);
	
	//operator
	const CTriChip& operator=(const CTriChip&);
};


//tessellated model
class AFX_EXT_CLASS CSTLModel: public CEntity
{
	DECLARE_SERIAL(CSTLModel)
//attribs
public:
	CTypedPtrArray<CObArray,CTriChip*>	m_TriList;

public:
	//constructor && destructor
	CSTLModel();
	virtual ~CSTLModel();

	//display
	void	Draw(COpenGLDC* pDC);

	//serialize
	virtual void Serialize(CArchive& ar);

	//		operation
	const   CSTLModel& operator=(const CSTLModel&);
	
	void	Add(CTriChip* tri);
	BOOL	LoadSTLFile(LPCTSTR file);
	void	Clear();

	//attrib accessing
	BOOL	IsEmpty();

	//get entity type
	virtual CEntity::Type type() const;

protected:
	virtual void UpdateBox();
};

class AFX_EXT_CLASS CBezierCurve : public CEntity
{
	DECLARE_SERIAL(CBezierCurve)
//attribs
protected:
	int	m_uSize;					//the number of control points
	CPoint3D*	m_ctrlPts;

public:
	//constructor && destructor
	CBezierCurve();
	CBezierCurve(CPoint3D* ctrlPts,int uSize);
	virtual ~CBezierCurve();

	//static
	static BOOL ReadDefinitionFile(FILE* file,CEntity*& pEnt);

	//edit
	void clear();
	void set(CPoint3D* ctrlPts,int uSize);

	//display
	virtual void Draw(COpenGLDC* pDC);

	//serialize
	virtual void Serialize(CArchive& ar);

	//attrib accessing
	BOOL	IsEmpty() const;

	//get entity type
	virtual CEntity::Type type() const;

protected:
	virtual void UpdateBox();
};

class AFX_EXT_CLASS CBezierSurface : public CEntity
{
	DECLARE_SERIAL(CBezierSurface)
	//attribs
protected:
	int				m_uSize;			//the number of control points
	int				m_vSize;			//the number of control points
	CPoint3D*	m_ctrlPts;

public:
	//constructor && destructor
	CBezierSurface();
	CBezierSurface(CPoint3D* ctrlPts,	int uSize, int vSize);
	virtual ~CBezierSurface();

	//static
	static BOOL ReadDefinitionFile(FILE* file,CEntity*& pEnt);

	//edit
	void clear();
	void set(CPoint3D* ctrlPts, int uSize,	int vSize);

	//display
	virtual void Draw(COpenGLDC* pDC);

	//serialize
	virtual void Serialize(CArchive& ar);

	//attrib accessing
	BOOL	IsEmpty() const;

	//get entity type
	virtual CEntity::Type type() const;

protected:
	virtual void UpdateBox();
};

class AFX_EXT_CLASS CNurbsCurve : public CEntity
{
	DECLARE_SERIAL(CNurbsCurve)
	//attribs
public:
	int	m_uOrder;			
	int	m_uSize;			//	the number of control points
	CPoint3D*	m_ctrlPts;
	float*		m_uKnots;

public:
	//constructor && destructor
	CNurbsCurve();
	CNurbsCurve(CPoint3D* ctrlPts,int uSize, int uOrder,
							float* uKnots= NULL);
	virtual ~CNurbsCurve();

	//static
	static BOOL ReadDefinitionFile(FILE* file,CEntity*& pEnt);

	//edit
	void clear();
	void set(CPoint3D* ctrlPts, int uSize, int uOrder, float* uKnots= NULL);

	//display
	virtual void Draw(COpenGLDC* pDC);

	//serialize
	virtual void Serialize(CArchive& ar);

	//attrib accessing
	BOOL	IsEmpty() const;

	//get entity type
	virtual CEntity::Type type() const;

protected:
	virtual void UpdateBox();
};

class AFX_EXT_CLASS CNurbsSurface : public CEntity
{
	DECLARE_SERIAL(CNurbsSurface)
	//attribs
public:
	int	m_uOrder;
	int	m_vOrder;
	int	m_uSize;
	int	m_vSize;
	CPoint3D* m_ctrlPts;
	float* m_uKnots;
	float* m_vKnots;

public:
	//constructor && destructor
	CNurbsSurface();
	CNurbsSurface(CPoint3D* ctrlPts,int uSize,int vSize,
								int uOrder,int vOrder,
								float* uknots = NULL,
								float* vKnots = NULL);
	virtual ~CNurbsSurface();

	//static
	static BOOL ReadDefinitionFile(FILE* file,CEntity*& pEnt);

	//edit
	void clear();
	void set(CPoint3D* vex,int uSize, int vSize,
								int uOrder,int vOrder,
								float* uknots = NULL,
								float* vKnots = NULL);

	//display
	virtual void Draw(COpenGLDC* pDC);

	//serialize
	virtual void Serialize(CArchive& ar);

	//attrib accessing
	BOOL	IsEmpty() const;
	
	//get entity type
	virtual CEntity::Type type() const;

protected:
	virtual void UpdateBox();
};

////////////////////////////////////////////////////////
class AFX_EXT_CLASS CPart : public CEntity
{
//attribs
protected:
	CTypedPtrArray<CObArray,CEntity*> m_EntList;

public:
	//constructor && destructor
	CPart();
	virtual ~CPart();

	//draw
	virtual void Draw(COpenGLDC* pDC);

	//serialize
	virtual void Serialize(CArchive& ar);

	//operation
	void	AddEntity(CEntity* ent);
	void	RemoveEntity(CEntity* ent);
	void	RemoveAllEntity();

	//attrib accessing
	BOOL	IsEmpty();

	UINT		GetEntitySize();
	CEntity*	GetEntity(UINT i);

	//get entity type
	virtual CEntity::Type type() const;

protected:
	virtual void	UpdateBox();
};