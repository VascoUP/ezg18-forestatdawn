#pragma once

class IRenderable
{
private:
	int m_index = 0;
public:
	IRenderable() { };

	void SetIndex(int index) { m_index = index; }
	int GetIndex() const { return m_index; }

	virtual void Load() = 0;
	virtual void Render() = 0;
	virtual void Clear() = 0;

	~IRenderable() {};
};