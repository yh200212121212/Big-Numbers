#pragma once

#include <D3DGraphics/D3Scene.h>
#include <D3DGraphics/MeshBuilder.h>

#define BOARDHEIGHT 0.8f
#define BOARDSIZE   16
#define HALFSIZE    (BOARDSIZE/2)
#define GAMECOLOR   D3DCOLOR_XRGB(235,235,122)

class BoardFieldObject : public SceneObjectWithMesh {
private:
  static LPDIRECT3DTEXTURE s_unmarkedTexture;
  static LPDIRECT3DTEXTURE s_markTexture;
  D3DXVECTOR3              m_center;
  bool                     m_selected;
  LPDIRECT3DTEXTURE getTexture(bool marked);
  static LPD3DXMESH createMesh(LPDIRECT3DDEVICE device, int row, int col);
public:
  BoardFieldObject(D3Scene &scene, int row, int col);
  D3PosDirUpScale getPDUS() const {
    return D3Scene::getOrigo();
  }
  inline void setSelected(bool selected) {
    m_selected = selected;
#ifdef _DEBUG
    if(selected) {
      debugLog(_T("Select %s\n"), getName().cstr());
    }
#endif
  }
  inline const D3DXVECTOR3 &getCenter() const {
    return m_center;
  }
  void draw();
#ifdef _DEBUG
  String toString() const;
#endif
};

class BrickObject : public SceneObjectWithMesh {
private:
  D3PosDirUpScale   m_pdus;
  D3SceneObject    *m_brickMarker;
  bool              m_marked;
  static LPD3DXMESH createMesh(LPDIRECT3DDEVICE device, BYTE attr);
public:
  BrickObject(D3Scene &scene, BYTE attr);
  ~BrickObject();
  D3PosDirUpScale getPDUS() const {
    return m_pdus;
  }
  inline void setPos(const D3DXVECTOR3 &pos) {
    m_pdus.setPos(pos);
#ifdef _DEBUG
    debugLog(_T("New position(%s) %s\nPDUS:\n%s")
            ,getName().cstr()
            ,::toString(pos).cstr()
            ,indentString(getPDUS().toString(),2).cstr());
#endif
  }
  inline void setMarked(bool marked) {
    m_marked = marked;
#ifdef _DEBUG
    if (marked) {
      debugLog(_T("Mark %s\n"), getName().cstr());
    }
#endif
  }
  void draw();
#ifdef _DEBUG
  String toString() const;
#endif
};

class GameBoardObject : public SceneObjectWithMesh {
private:
  static const float s_vertexPos[];
  static float       s_gridLines[5];
  LPDIRECT3DTEXTURE  m_boardTexture;
  BoardFieldObject  *m_fieldObject[ROWCOUNT][COLCOUNT];
  BrickObject       *m_brickObject[FIELDCOUNT];
  Field              m_currentField;
  char               m_currentBrick;
  static LPD3DXMESH createMesh(LPDIRECT3DDEVICE device);
  friend class BoardFieldObject ;
  void setCurrentFieldSelected(bool selected);
  void setCurrentBrickSelected(bool selected);
public:
  GameBoardObject(D3Scene &scene);
  ~GameBoardObject();
  inline D3PosDirUpScale getPDUS() const {
    return D3Scene::getOrigo();
  }
  inline bool hasCurrentField() const {
    return m_currentField.isField();
  }
  inline bool hasCurrentBrick() const {
    return m_currentBrick != NOBRICK;
  }
  D3DXVECTOR3 getFieldCenter(const Field &f) const {
    return m_fieldObject[f.m_row][f.m_col]->getCenter();
  }

  void resetBrickPositions(bool colored);
  void setBrickOnField(BYTE brick, const Field &f);
  void markField(const Field &f);
  void unmarkCurrentField();
  void markBrick(char brick);
  void unmarkCurrentBrick();
  void setBricksVisible(BrickSet set, bool visible);
  const Field getCurrentField() const {
    return m_currentField;
  }
  char getCurrentBrick() const {
    return m_currentBrick;
  }
  void draw();
  int   getBrickFromPoint(const CPoint &p) const;
  Field getFieldFromPoint(const CPoint &p) const;
#ifdef _DEBUG
  String toString() const;
#endif
};
