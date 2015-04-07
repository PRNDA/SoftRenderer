/**
  @file SrTexture.h
  
  @author yikaiming

  ������־ history
  ver:1.0
   
 */

#ifndef SrTexture_h__
#define SrTexture_h__
#include "prerequisite.h"
#include "SrResource.h"

enum ESrBitmapType
{
	eBt_file,
	eBt_internal,
};

class SrTexture : public SrResource
{
public:
	SrTexture( const char* name ) : SrResource(name, eRT_Texture), m_userData(NULL)
	{
		GtLog("[ResourceManager] Texture[%s] Created.", m_name.c_str());
	}
	virtual ~SrTexture(void)
	{

	}
	const uint8* getBuffer() const {return m_data;}
	int getBPP() const {return m_bpp;}
	int getWidth() const {return m_width;}
	int getHeight() const {return m_height;}  
	int getPitch() const {return m_pitch;}
	ESrBitmapType getTextureType() const {return m_texType;}

	void Apply(uint8 stage, uint8 samplertype) const
	{
		gEnv->renderer->SetTextureStage( this, stage);
	}

	uint32 Get( int2& p ) const
	{
		uint32 ret = 0;
		uint8* data = m_data;

		assert( p.x >=0 && p.x < m_width );
		assert( p.y >=0 && p.y < m_height );

		if ( m_bpp == 3 )
		{
			ret = uint8BGR_2_uint32(data + (m_bpp * p.x + p.y * m_pitch));
			//ret = *(uint32*)(data + (m_bpp * p.x + p.y * m_pitch));
		}
		else
		{
			//ret = uint8BGRA_2_uint32(data + (m_bpp * p.x + p.y * m_pitch));
			ret = *(uint32*)(data + (m_bpp * p.x + p.y * m_pitch));
		}
		
		return ret;
	}
	

	uint32 Get( const float2& p, ESamplerFilter mode /*= eSF_Linear*/ ) const
	{
		uint32 final = 0x00000000;

		// ��������warp
		float u = p.x - floor(p.x);
		float v = p.y - floor(p.y);

		switch(mode)
		{
		case eSF_Nearest:
			{
				// �ٽ������
				u *= (m_width);
				v *= (m_height);

				// get int
				int x = (int)( u );
				int y = (int)( v );

				x = x % m_width;
				y = y % m_height;

				assert( x >= 0 && x < m_width );
				assert( y >= 0 && y < m_height );

				final = Get(int2(x,y));
			}

			break;
		case eSF_Linear:
			// ˫�����ڲ�
		default:
			{
				// ȡ������ռ�ĵ�ַ
				u *= (m_width - 1);
				v *= (m_height - 1);

				// ȡ��
				int x = (int)( u );
				int y = (int)( v );

				// ����������ƫ��
				float du = u - x;
				float dv = v - y;

				// ȡ�õ�ַλ�ã�ȷ�������������Χ��
				int left = (x) % m_width;
				int right = (x + 1) % m_width;
				int up = (y) % m_height;
				int down = (y + 1) % m_height;

				assert( left >= 0 && left < m_width );
				assert( right >= 0 && right < m_width );
				assert( up >= 0 && up < m_height );
				assert( down >= 0 && down < m_height );

				// ����4����ɫ
				uint32 lt = Get( int2(left,up) );
				uint32 t = Get( int2(right,up) );
				uint32 l = Get( int2(left,down) );
				uint32 rb =  Get( int2(right,down) );

				// ʹ���ĸ���ɫ��������ƫ��������������ɫ
				final = SrColorMulFloat(lt,((1.f-du)*(1.f-dv))) + SrColorMulFloat(t , (du*(1.f - dv))) + SrColorMulFloat(l , ((1.f-du)*dv)) + SrColorMulFloat(rb , (du*dv));
			}
		}
		return final;
	}

	mutable void*	m_userData;

protected:
	SrTexture(void);

	int m_width;
	int m_height;
	int m_bpp;
	int m_pitch;

	uint8*	m_data;
	ESrBitmapType m_texType;
	
};

#endif // SrTexture_h__


