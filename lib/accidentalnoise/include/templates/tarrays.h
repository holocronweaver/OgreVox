#ifndef TARRAYS_H
#define TARRAYS_H
#include <vector>
#include <stdexcept>
#include <cmath>
#include "../vectortypes.h"

namespace anl
{
enum ERasterTriangleModes
{
    RTM_Replace,
    RTM_Add,
    RTM_SubtractSrc,
    RTM_SubtractDest,
    RTM_Average,
    RTM_AverageNonZero,
    RTM_Min,
    RTM_Max,
    RTM_Multiply,
    RTM_Blend
};
/*template<class T> class TArray2D
{
    public:
    TArray2D() : m_width(0), m_height(0){}
    TArray2D(size_t width, size_t height) { resize(width,height);}
    ~TArray2D(){}

    size_t width(){return m_width;}
    size_t height(){return m_height;}

    void resize(size_t width, size_t height)
    {
        m_array.resize(width*height);
        m_width=width;
        m_height=height;
        for(size_t c=0; c<m_width*m_height; ++c) m_array[c]=T(0);
    }

    void set(size_t x, size_t y, T val)
    {
        if(x>=m_width || y>=m_height) throw(std::out_of_range("Array index out of range"));
        m_array[y*m_width+x]=val;
    }

    T get(size_t x, size_t y)
    {
        if(x>=m_width || y>=m_height) throw(std::out_of_range("Array index out of range"));
        return m_array[y*m_width+x];
    }

    TArray2D &operator=(const TArray2D<T> &v)
    {
        resize(v.width(), v.height());
        for(size_t c=0; c<m_width*m_height; ++c)
        {
            m_array[c]=v.m_array[c];
        }
        return *this;
    }

    TArray2D &operator=(const T& v)
    {
        for(size_t c=0; c<m_width*m_height; ++c) m_array[c]=v;
    }

    protected:
    size_t m_width, m_height;
    std::vector<T> m_array;



}; */

template <class T>
class TArray2D
{
    private:
        T *m_data;
        int m_width, m_height;

        void swapPairs(CVec2i &vec1, T &val1, CVec2i &vec2, T &val2)
        {
            std::swap(vec1, vec2);
            std::swap(val1,val2);
        };



        T calcRasterValue(T vsrc, T vdest, float blend, int mode)
            {
            switch(mode)
            {
                case RTM_Replace: return vsrc; break;
                case RTM_Add: return vsrc+vdest; break;
                case RTM_SubtractSrc: return (vdest-vsrc); break;
                case RTM_SubtractDest: return (vsrc-vdest); break;
                case RTM_Average: return (T)((vsrc+vdest)*0.5f); break;
                //case RTM_Min: if(vdest<vsrc) return vdest; else return vsrc; break;
                //case RTM_Max: if(vdest>vsrc) return vdest; else return vsrc; break;
                case RTM_Multiply: return (vdest*vsrc); break;
                case RTM_Blend: return vdest + (T)((vsrc-vdest)*blend);
                default: return vsrc; break;
            }
        }



        void flatTopTriangle(CVec2i vec1, T v1, CVec2i vec2,
                                    T v2, CVec2i vec3, T v3, int mode)
        {

	       if (!m_data) return;
	       if(vec1==vec2 || vec2==vec3 || vec1==vec3) return;

           int numrows=(vec3.y()-vec1.y()) + 1;
	       if(numrows<0) numrows = -numrows;
	       if(vec2.x()<vec1.x()) swapPairs(vec1,v1,vec2,v2);

	       float s=0.0f;
	       float sinc=1.0f / (float)(numrows-1);

	       for(int c=0; c<numrows; ++c)
	       {
                int lx=(int)((float)vec1.x()+s*(float)(vec3.x()-vec1.x()));
                int rx=(int)((float)vec2.x()+s*(float)(vec3.x()-vec2.x()));

                T lv = v1 + (T)((v3-v1)*s);
                T rv = v2 + (T)((v3-v2)*s);

                for (int x=lx; x<=rx; ++x)
                {
                    int y=vec1.y()+c;
                    T v;
                    float t;
                    if(rx!=lx)
                    {
                        t=((float)x-(float)lx) / (((float)rx-(float)lx));
                        v = lv + (T)((rv-lv)*t);
                        T vdest=get(x,y);
                        set(x,y,calcRasterValue(v,vdest,0.0f,mode));
                    }
                }
                s+=sinc;
            }
        };

        void flatBottomTriangle(CVec2i vec1, T v1, CVec2i vec2, T v2,
            CVec2i vec3, T v3, int mode)
        {
            if(vec1==vec2 || vec2==vec3 || vec1==vec3) return;

            int numrows=(vec3.y()-vec1.y())+1;
            if(numrows<0) numrows = -numrows;
            if(vec3.x()<vec2.x()) swapPairs(vec2, v2, vec3, v3);

            float s=0.0f;
            float sinc=1.0f / (float)(numrows-1);

            for(int c=0; c<numrows; ++c)
            {
                int lx=(int)((float)vec1.x()+s*(float)(vec2.x()-vec1.x()));
                int rx=(int)((float)vec1.x()+s*(float)(vec3.x()-vec1.x()));

                T lv = v1 + (T)((v2-v1)*s);
                T rv = v1 + (T)((v3-v1)*s);

                for(int x=lx; x<=rx; ++x)
                {
                    int y=vec1.y()+c;
                    T v;
                    float t;
                    if(rx!=lx)
                    {
                        t=((float)x-(float)lx) / (((float)rx-(float)lx));
                        v = lv + (T)((rv-lv)*t);
                        T vdest=get(x,y);
                        set(x,y,calcRasterValue((T)v,vdest,0.0f,mode));
                    }
                }
                s+=sinc;
            }
        };

        void flatTopTriangleTex(TArray2D<T> *src1,
            CVec4i vec1, CVec4i vec2, CVec4i vec3, int mode)
        {
            if(!m_data) return;
            if(!src1) return;
           // if(mode==RTM_Blend && !blend) return;

            if(vec1==vec2 || vec2==vec3 || vec1==vec3) return;

            int numrows=(vec3.y()-vec1.y())+1;
            if(numrows<0) numrows=-numrows;
            if(vec2.x()<vec1.x()) std::swap(vec2,vec1);
            float s=0.0f;
            float sinc=1.0f / (float) (numrows-1);

            for(int c=0; c<numrows; ++c)
            {
                int lx=(int)((float)vec1.x()+s*(float)(vec3.x()-vec1.x()));
                int rx=(int)((float)vec2.x()+s*(float)(vec3.x()-vec2.x()));
                float lu=((float)vec1.z()+s*((float)vec3.z()-(float)vec1.z()));
                float ru=((float)vec2.z()+s*((float)vec3.z()-(float)vec2.z()));
                float lv=((float)vec1.w()+s*((float)vec3.w()-(float)vec1.w()));
                float rv=((float)vec2.w()+s*((float)vec3.w()-(float)vec2.w()));

                for(int x=lx; x<=rx; ++x)
                {
                    int y=vec1.y()+c;
                    T v;
                    float t;
                    if(rx!=lx)
                    {
                        t=((float)x-(float)lx) / (((float)rx-(float)lx));
                        float ufinal = ((float)lu + t*((float)ru-(float)lu));
                        float vfinal = ((float)lv + t*((float)rv-(float)lv));
                        src1->wrapCoords(ufinal, vfinal);
                        T vdest=get(x,y);
                        v = src1->get(ufinal, vfinal);

                        /*float b;
                        if(mode==RTM_Blend)
                        {
                            b=blend->get(ufinal,vfinal);
                        }else
                        {
                            b=0.0f;
                        }*/

                        set(x,y,calcRasterValue((T)v,vdest,0.0f,mode));
                    }
                }
                s+=sinc;
            }
        };

        void flatBottomTriangleTex(TArray2D<T> *src1,
            CVec4i vec1, CVec4i vec2, CVec4i vec3, int mode)
        {
            if(!m_data) return;
            if(!src1) return;
            //if(mode==RTM_Blend && !blend) return;
            if(vec1==vec2 || vec2==vec3 || vec1==vec3) return;

            int numrows=(vec3.y()-vec1.y())+1;
            if(numrows<0) numrows = -numrows;
            if(vec3.x()<vec2.x()) std::swap(vec2, vec3);

            float s=0.0f;
            float sinc=1.0f / (float)(numrows-1);

            for(int c=0; c<numrows; ++c)
            {
                int lx=(int)((float)vec1.x()+s*(float)(vec2.x()-vec1.x()));
                int rx=(int)((float)vec1.x()+s*(float)(vec3.x()-vec1.x()));
                float lu=((float)vec1.z()+s*((float)vec2.z()-(float)vec1.z()));
                float ru=((float)vec1.z()+s*((float)vec3.z()-(float)vec1.z()));
                float lv=((float)vec1.w()+s*((float)vec2.w()-(float)vec1.w()));
                float rv=((float)vec1.w()+s*((float)vec3.w()-(float)vec1.w()));

                for(int x=lx; x<=rx; ++x)
                {
                    int y=vec1.y()+c;
                    T v;
                    float t;
                    if(rx!=lx)
                    {
                        t=((float)x-(float)lx) / (((float)rx-(float)lx));
                        float ufinal = ((float)lu + t*((float)ru-(float)lu));
                        float vfinal = ((float)lv + t*((float)rv-(float)lv));
                        src1->wrapCoords(ufinal, vfinal);
                        T vdest=get(x,y);
                        v = src1->get(ufinal, vfinal);

                        /*float b;
                        if(mode==RTM_Blend)
                        {
                            b=blend->get(ufinal, vfinal);
                        }
                        else
                        {
                            b=0.0f;
                        }*/

                        set(x,y,calcRasterValue((T)v,vdest,0.0f,mode));
                    }
                }
                s+=sinc;
            }
        };


    public:
        TArray2D(int w, int h) : m_data(0), m_width(w), m_height(h)
        {
            resize(w,h);
        }
        TArray2D(){m_width=0; m_height=0; m_data=0;};
        ~TArray2D(){
            destroy();
        };

        void resize(int w, int h)
        {
            destroy();
            if(w==0 || h==0) return;
            m_data = new T[w*h];
            m_width=w;
            m_height=h;
            fill((T)(0));
        };

        void destroy()
        {
            if(m_data) delete[] m_data;
            m_data=0;
            m_width=0;
            m_height=0;
        };

        int width(){return m_width;};
        int height(){return m_height;};
        T *getData(){return m_data;};   // Hackification

        void set(int x, int y, T v)
        {
            if(x>=m_width || y>=m_height || x<0 || y<0) return;
            if(!m_data) return;

            m_data[y*m_width+x]=v;
        }

        void set(unsigned int x, unsigned int y, T v)
        {
            if((int)x>=m_width || (int)y>=m_height) return;
            if(!m_data) return;

            m_data[y*m_width+x]=v;
        }


        T get(int x, int y)
        {
            if(x>=m_width || y>=m_height || !m_data) return T(0);

            return m_data[y*m_width+x];
        }

        T get(unsigned int x, unsigned int y)
        {
            if(x>=m_width || y>=m_height || !m_data) return T(0);

            return m_data[y*m_width+x];
        }

        T &getRef(int x, int y)
        {
            //if(x>=m_width || y>=m_height || !m_data) return T(0);

            return m_data[y*m_width+x];
        }

        T get(float x, float y)
        {
            int ix = (int)x;
            int iy = (int)y;
            int nx = ix+1;
            int ny = iy+1;

            if(ix>=m_width || iy>=m_height) return T(0);
            if(nx>=m_width) nx=ix;
            if(ny>=m_height) ny=iy;

            T v1=get(ix,iy);
            T v2=get(nx,iy);
            T v3=get(ix,ny);
            T v4=get(nx,ny);

            float s = x - (float)ix;
            float t = y - (float)iy;

            // interpolate v1->v2 and v3->v4 on s
            T v5 = v1 + (T)((v2-v1)*s);
            T v6 = v3 + (T)((v4-v3)*s);

            // interpolated v5->v6 on t
            T final = v5 + (T)((v6-v5)*t);
            return final;
        }

        T getIndexed(int c)
        {
            if(c >= m_height*m_width + m_width || c<0) return T(0);
            return m_data[c];
        }

        void setIndexed(int c, T v)
        {
            if(c>=m_height*m_width+m_width || c<0) return;
            m_data[c]=v;
        }

        void fill(T v){
            if(!m_data) return;

            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    m_data[y*m_width+x]=v;
                }
            }
        }

        void copyFrom(TArray2D<T> *b)
        {
            if(!m_data) return;
            int w,h;

            if(!b) return;
            w=b->width();
            h=b->height();
            if(w != m_width || h != m_height) return;  // Must be same size

            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    set(x,y,b->get(x,y) );
                }
            }
        }

        void copyFromSub(TArray2D<T> *b, int x, int y)
        {
            if(!m_data) return;
            int w,h;

            if(!b) return;
            w=b->width();
            h=b->height();
            if(w==0 || h==0 ) return;
            for(int cx=0; cx<w; ++cx)
            {
                for(int cy=0; cy<h; ++cy)
                {
                    set(x+cx, y+cy, b->get(cx,cy));
                }
            }
        }

        void addArray(TArray2D<T> *b)
        {
            if(!m_data) return;
            int w,h;

            if(!b) return;
            w=b->width();
            h=b->height();
            if(w != m_width || h != m_height) return;  // Must be same size

            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    T temp = get(x,y);
                    set(x,y,b->get(x,y)+temp );
                }
            }
        }

        void addArraySub(TArray2D<T> *b, int x, int y)
        {
            if(!m_data) return;
            int w,h;

            if(!b) return;
            w=b->width();
            h=b->height();
            if(w==0 || h==0) return;

            for(int cx=0; cx<w; ++cx)
            {
                for(int cy=0; cy<h; ++cy)
                {
                    T temp = get(cx+x, cy+y);
                    set(cx+x, cy+y, b->get(cx,cy)+temp);
                }
            }
        }

        void subtractArray(TArray2D<T> *b)
        {
            if(!m_data) return;
            int w,h;

            if(!b) return;
            w=b->width();
            h=b->height();
            if(w != m_width || h != m_height) return;  // Must be same size

            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    T temp = get(x,y);
                    set(x,y,temp-b->get(x,y) );
                }
            }
        }

        void subtractArraySub(TArray2D<T> *b, int x, int y)
        {
            if(!m_data) return;
            int w,h;

            if(!b) return;
            w=b->width();
            h=b->height();
            if(w==0 || h==0) return;

            for(int cx=0; cx<w; ++cx)
            {
                for(int cy=0; cy<h; ++cy)
                {
                    T temp = get(cx+x, cy+y);
                    set(cx+x, cy+y, temp-b->get(cx,cy));
                }
            }
        }
        void multiplyArray(TArray2D<T> *b)
        {
            if(!m_data) return;
            int w,h;

            if(!b) return;
            w=b->width();
            h=b->height();
            if(w != m_width || h != m_height) return;  // Must be same size

            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    T temp = get(x,y);
                    set(x,y,b->get(x,y)*temp );
                }
            }
        }

        void multiplyArraySub(TArray2D<T> *b, int x, int y)
        {
            if(!m_data) return;
            int w,h;

            if(!b) return;
            w=b->width();
            h=b->height();
            if(w==0 || h==0) return;

            for(int cx=0; cx<w; ++cx)
            {
                for(int cy=0; cy<h; ++cy)
                {
                    T temp = get(cx+x, cy+y);
                    set(cx+x, cy+y, b->get(cx,cy)*temp);
                }
            }
        }

        void scale(T s)
        {
            if(!m_data) return;
            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    T temp=get(x,y);
                    set(x,y,temp*s);
                }
            }
        }

        void blendFromArrays(TArray2D<T> *b1, TArray2D<T> *b2, TArray2D<float> *b3)
        {
            if(!m_data) return;
            if(!b1 || !b2 || !b3) return;
            if(b1->width() != m_width || b2->width() != m_width || b3->width() != m_width) return;
            if(b1->height() != m_height || b2->height() != m_height || b3->height() != m_height) return;

            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    float t=b3->get(x,y);
                    T v1=b1->get(x,y);
                    T v2=b2->get(x,y);
                    //T val = (T) ( (float)v1*(1-t) + (float)v2*t );
                    T val = v1 + (T)((v2-v1)*t);
                    set(x,y,val);
                }
            }
        }

        void selectFromArrays(TArray2D<T> *b1, TArray2D<T> *b2, TArray2D<float> *b3, float threshold)
        {
            if(!m_data) return;
            if(!b1 || !b2 || !b3) return;
            if(b1->width() != m_width || b2->width() != m_width || b3->width() != m_width) return;
            if(b1->height() != m_height || b2->height() != m_height || b3->height() != m_height) return;

            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    float t=b3->get(x,y);
                    if(t<threshold) set(x,y,b1->get(x,y));
                    else set(x,y,b2->get(x,y));
                }
            }
        }

        T getMax()
        {
            if(!m_data) return (T)0;
            T m = m_data[0];
            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    T v=get(x,y);
                    if(v > m) m=v;
                }
            }

            return m;
        }

        T getMin()
        {
            if(!m_data) return (T)0;
            T m = m_data[0];
            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    T v=get(x,y);
                    if(v < m) m=v;
                }
            }

            return m;
        }

        void scaleToRange(T low, T high)
        {
            if(!m_data) return;
            T max=getMax();
            T min=getMin();

            for(int x=0; x<m_width; ++x)
            {
                for(int y=0; y<m_height; ++y)
                {
                    T temp = get(x,y);
                    temp = temp - min;
                    //float ftemp = (float)temp / ( (float)max - (float)min);
                    T ftemp = temp / (max-min);
                    T val = ftemp * (high-low);
                    //T val = (T)(ftemp * ((float)high-(float)low));
                    val = val + low;
                    set(x,y,val);
                }
            }
        }

        void wrapCoords(int &x, int &y)
        {
            if(m_width==0 || m_height==0) return;
            while(x >= m_width) x -= m_width;
            while(y >= m_height) y -= m_height;
        }

        void wrapCoords(float &x, float &y)
        {
            if(m_width==0 || m_height==0) return;
            while(x>=(float)m_width) x-=(float)m_width;
            while(y>=(float)m_height) y-=(float)m_height;
        }

		void offset(int ox, int oy)
		{
			if(!m_data) return;
			// Offset buffer
			TArray2D<T> tempbuf;
			tempbuf.resize(m_width, m_height);
			for(int x=0; x<m_width; ++x)
			{
				for(int y=0; y<m_height; ++y)
				{
					int nx=x-ox;
					int ny=y-oy;
					wrapCoords(nx,ny);
					tempbuf.set(x,y,get(nx,ny));
				}
			}
			copyFrom(&tempbuf);
		}

		void flipVertical()
		{
			if(!m_data) return;
			for(int x=0; x<m_width;++x)
			{
				for(int y=0; y<m_height; ++y)
				{
					T c1=get(x,y);
					T c2=get(x,(m_height-1)-y);
					set(x,y,c2);
					set(x,(m_height-1)-y,c1);
				}
			}
		}

		void flipHorizontal()
		{
			if(!m_data) return;
			for(int x=0; x<m_width;++x)
			{
				for(int y=0; y<m_height; ++y)
				{
					T c1=get(x,y);
					T c2=get((m_width-1)-x,y);
					set(x,y,c2);
					set((m_width-1)-x,y,c1);
				}
			}
		}

        void rasterTriangleSmooth(CVec2i vec1, T v1, CVec2i vec2, T v2,
            CVec2i vec3, T v3, int mode)
        {
            if(vec2.y()<vec1.y())
            {
                swapPairs(vec1,v1,vec2,v2);
            }

            if(vec3.y()<vec1.y())
            {
                swapPairs(vec1,v1,vec3,v3);
            }

            if(vec3.y() < vec2.y())
            {
                swapPairs(vec3,v3,vec2,v2);
            }

            if(vec1.y()==vec2.y())
            {
                flatTopTriangle(vec1,v1,vec2,v2,vec3,v3,mode);
                return;
            }

            if(vec2.y()==vec3.y())
            {
                flatBottomTriangle(vec1,v1,vec2,v2,vec3,v3,mode);
                return;
            }

            float t = ((float)vec2.y()-(float)vec1.y())/((float)vec3.y()-(float)vec1.y());
            int new_x=(int) ((float)vec1.x()+(((float)vec3.x()-(float)vec1.x()) * t));
            T new_v = (T)(v1 + ((v3-v1) * (t)));

            CVec2i newvec(new_x,vec2.y());
            flatBottomTriangle(vec1,v1,newvec,new_v, vec2,v2, mode);
            flatTopTriangle(vec2, v2, newvec,new_v, vec3,v3, mode);
            return;
        }

    void rasterTriangleTex(TArray2D<T> *src1,
        CVec4i vec1, CVec4i vec2, CVec4i vec3, int mode)
    {
        if(vec2.y()<vec1.y())
        {
            std::swap(vec1,vec2);
        }

        if(vec3.y()<vec1.y())
        {
            std::swap(vec3,vec1);
        }

        if(vec3.y()<vec2.y())
        {
            std::swap(vec3,vec2);
        }

        if(vec1.y()==vec2.y())
        {
            flatTopTriangleTex(src1,vec1,vec2,vec3,mode);
            return;
        }

        if(vec2.y()==vec3.y())
        {
            flatBottomTriangleTex(src1,vec1,vec2,vec3,mode);
            return;
        }

        int new_x=(int)((float)vec1.x()+(((float)vec2.y()-(float)vec1.y())*((float)vec3.x()-(float)vec1.x())/((float)vec3.y()-(float)vec1.y())));
        int new_u=(int)((float)vec1.z()+(((float)vec2.y()-(float)vec1.y())*((float)vec3.z()-(float)vec1.z())/((float)vec3.y()-(float)vec1.y())));
        int new_v=(int)((float)vec1.w()+(((float)vec2.y()-(float)vec1.y())*((float)vec3.w()-(float)vec1.w())/((float)vec3.y()-(float)vec1.y())));

        CVec4i newvec(new_x, vec2.y(), new_u, new_v);
        flatBottomTriangleTex(src1,vec1,newvec,vec2,mode);
        flatTopTriangleTex(src1,vec2,newvec,vec3,mode);
    }

	T sampleRow(TArray2D<T> *src, TArray2D<double> *blur, float x, float y)
	{
		// Apply sample kernel to the rows of the source image to sample down
		int bw=blur->width();
		T total=T(0);
		float totalweight=0.0f;
		for(int xx=0; xx<bw; ++xx)
		{
			float ox=(float)(xx-bw/2);
			float nx=x+ox;
			if(nx>=0 && nx<src->width())
			{
				float k=(float)blur->get(xx,0);
				T v=src->get(nx,y);
				total=total+v*k;
				totalweight+=k;
			}
		}
		return total/totalweight;
	}

	T sampleCol(TArray2D<T> *src, TArray2D<double> *blur, float x, float y)
	{
		// Apply sample kernel to the rows of the source image to sample down
		int bw=blur->width();
		T total=T(0);
		float totalweight=0.0f;
		for(int yy=0; yy<bw; ++yy)
		{
			float oy=(float)(yy-bw/2);
			float ny=y+oy;
			if(ny>=0 && ny<src->height())
			{
				float k=(float)blur->get(yy,0);
				T v=src->get(x,ny);
				total=total+v*k;
				totalweight+=k;
			}
		}
		return total/totalweight;
	}

	void buildSampleKernel(TArray2D<double> *kernel, int size)
	{
		kernel->resize(size,1);
		double c=(double)size / 2.0;
		double total=0;
		for(int x=0; x<size; ++x)
		{
			double d=std::abs((double)x-c) / c;
			d=1.0-d;
			kernel->set(x,0,d);
			total+=d;
		}
		for(int x=0; x<size; ++x)
		{
			kernel->set(x,0,kernel->get(x,0)/total);
		}
	}

	void get4x4Neighborhood(T *block, int x, int y)
	{
		// Copy out a 4x4 block of values. Any values that go beyond the borders are clamped.
		// Neighborhood occupies the range [x-1,x+2][y-1,y+2]
		for(int xx=-1; xx<=2; ++xx)
		{
			for(int yy=-1; yy<=2; ++yy)
			{
				int bx=xx+1;
				int by=yy+1;

				int nx=x+xx;
				if(nx<0) nx=0;
				if(nx>=m_width) nx=m_width-1;

				int ny=y+yy;
				if(ny<0) ny=0;
				if(ny>=m_height) ny=m_height-1;

				block[by*4+bx]=get(nx,ny);
			}
		}
	}

	T splineVector(T *vec, float parm)
	{
		T c = (vec[2]-vec[0]) * 0.5f;
		T v = (vec[1]-vec[2]);
		T w = c+v;
		T a = w+v+(vec[3]-vec[2])*0.5f;
		T b = w+a;
		return ((((a*parm)-b)*parm + c) * parm + vec[2]);

		// Catmull-Rom Interpolation
		/*
		float t=parm;
		float t2=parm*parm;
		float t3=t2*parm;
		return ((vec[1]*2.0f)+(vec[2]-vec[1])*t+(vec[0]*2.0f - vec[1]*5.0f + vec[2]*4.0f - vec[3])*t2 + (vec[1]*3-vec[2]*3-vec[0]+vec[3])*t3)*0.5f;
*/

	}

	T splineNeighborhood(T *block, float s, float t)
	{
		// Apply spline interpolation to neighborhood
		T a[4];
		for(int y=0; y<4; ++y)
		{
			// Spline the rows
			a[y]=splineVector(&block[y*4], s);
		}
		// Spline the columns
		return splineVector(a,t);
	}
	void scaleTo(TArray2D<T> *dst)
	{
		if(!dst || !m_data) return;
		int mw=dst->width(), mh=dst->height();
		T neighborhood[16];

		for(int x=0; x<mw; ++x)
		{
			for(int y=0; y<mh; ++y)
			{
				float s=(float)x/(float)mw;
				float t=(float)y/(float)mh;
				float nx=s*(float)m_width;
				float ny=t*(float)m_height;

				float p=nx - floorf(nx);
				float q=ny - floorf(ny);
				get4x4Neighborhood(neighborhood, (int)nx, (int)ny);
				T val=splineNeighborhood(neighborhood, p, q);
				dst->set(x,y,val);
			}
		}
	}

	void applyBlurRow(TArray2D<T> *src, TArray2D<T> *dest, TArray2D<double> *blur, int x, int y, bool seamless)
	{
		int bw=blur->width();
		T total=T(0);
		double totalweight=0;
		for(int xx=0; xx<bw; ++xx)
		{
			int nx=xx-bw/2;
			double k=blur->get(xx,0);
			T v;

			int cx=nx+x;
			if(seamless)
			{
				// Wrap coords
				//wrapCoords(cx,y);
				while(cx<0) cx+=m_width;
				while(cx>=m_width) cx-=m_width;
				totalweight+=k;
				v=src->get(cx,y);
			}
			else
			{
				if(cx>=0 && cx<src->width())
				{
					totalweight+=k;
					v=src->get(cx,y);
				}
				else v=T(0);
			}

			//T v=src->get(cx,y);
			total=total+(T)(v*k);

		}
		dest->set(x,y,(T)total/totalweight);
	}

	void applyBlurCol(TArray2D<T> *src, TArray2D<T> *dest, TArray2D<double> *blur, int x, int y, bool seamless)
	{
		int bw=blur->width();
		T total=T(0);
		double totalweight=0;
		for(int yy=0; yy<bw; ++yy)
		{
			int ny=yy-bw/2;
			int cy=y+ny;
			double k=blur->get(yy,0);
			T v;
			if(seamless)
			{
				//wrapCoords(x,cy);
				while(cy<0) cy+=m_height;
				while(cy>=m_height) cy-=m_height;
				totalweight+=k;
				v=src->get(x,cy);
			}
			else
			{
				if(cy>=0 && cy<src->height())
				{
					totalweight+=k;
					v=src->get(x,cy);
				}
				else
				{
					v=T(0);
				}
			}
			//T v=src->get(x,cy);
			total=total+(T)(v*k);

		}
		dest->set(x,y,(T)total/totalweight);
	}


	void blur(float blur_size, bool seamless)
	{
		if(!m_data) return;
		TArray2D<T> temp;
		temp.resize(m_width, m_height);
		TArray2D<double> kernelx, kernely;
		buildSampleKernel(&kernelx, (int)(blur_size*m_width));
		buildSampleKernel(&kernely, (int)(blur_size*m_height));

		for(int x=0; x<m_width;++x)
		{
			for(int y=0; y<m_height; ++y)
			{
				applyBlurRow(this, &temp, &kernelx, x, y, seamless);
			}
		}
		for(int x=0; x<m_width; ++x)
		{
			for(int y=0; y<m_height; ++y)
			{
				applyBlurCol(&temp, this, &kernely, x, y, seamless);
			}
		}
	}
};

template<class T> class TArray3D
{
    public:
    TArray3D() : m_width(0), m_height(0), m_depth(0){}
    TArray3D(size_t width, size_t height, size_t depth) { resize(width,height,depth);}
    ~TArray3D(){}

    size_t width(){return m_width;}
    size_t height(){return m_height;}
    size_t depth(){return m_depth;}

    void resize(size_t width, size_t height, size_t depth)
    {
        m_array.resize(width*height*depth);
        m_width=width;
        m_height=height;
        m_depth=depth;
        for(size_t c=0; c<m_width*m_height*depth; ++c) m_array[c]=T(0);
    }

    void set(size_t x, size_t y, size_t z, T val)
    {
        if(x>=m_width || y>=m_height || z>=m_depth) throw(std::out_of_range("Array index out of range"));
        m_array[z*(m_width*m_height)+y*m_width+x]=val;
    }

    T get(size_t x, size_t y, size_t z)
    {
        if(x>=m_width || y>=m_height || z>=m_depth) throw(std::out_of_range("Array index out of range"));
        return m_array[z*(m_width*m_height)+y*m_width+x];
    }

    TArray3D &operator=(const TArray2D<T> &v)
    {
        resize(v.width(), v.height(), v.depth());
        for(size_t c=0; c<m_width*m_height*m_depth; ++c)
        {
            m_array[c]=v.m_array[c];
        }
        return *this;
    }

    TArray3D &operator=(const T& v)
    {
        for(size_t c=0; c<m_width*m_height*m_depth; ++c) m_array[c]=v;
    }

    protected:
    size_t m_width, m_height, m_depth;
    std::vector<T> m_array;

};
};
#endif
