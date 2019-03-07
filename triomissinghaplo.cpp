#include "triomissinghaplo.H" 
triomissinghaplo::triomissinghaplo(const HaplotypeArray &d, int st, int len)
  :data(d.n(),len)
{
  assert(data.nhaps()%4==0);
  int ntrio=data.nhaps()/4;
  for (int trioCount=0;trioCount<ntrio;trioCount++) {
    int firsthap=trioCount*4;
    for (int j=0;j<len;j++) {
      int pos=st+j;
      // first look for the three patterns that effect all four values 
      switch (d[firsthap][pos]) {
      case hetquad:
	data[firsthap][j]=one;
	data[firsthap+1][j]=zero;
	data[firsthap+2][j]=zero;
	data[firsthap+3][j]=one;
	rock.push_back(setrocker(data[firsthap]+j,data[firsthap+1]+j
				     ,data[firsthap+2]+j,data[firsthap+3]+j));
	miss[std::pair<int,int>(firsthap,j)]=&rock.back();
	miss[std::pair<int,int>(firsthap+1,j)]=&rock.back();
	miss[std::pair<int,int>(firsthap+2,j)]=&rock.back();
	miss[std::pair<int,int>(firsthap+3,j)]=&rock.back();
	break;
      case hettrip:
	if (d[firsthap+1][pos]==hettrip) {
	  assert(d[firsthap+3][pos]==N);
	  data[firsthap][j]=one;
	  data[firsthap+1][j]=zero;
	  data[firsthap+2][j]=zero;
	  rock.push_back(setrocker(data[firsthap]+j,data[firsthap+1]+j
				       ,data[firsthap+2]+j));
	  miss[std::pair<int,int>(firsthap,j)]=&rock.back();
	  miss[std::pair<int,int>(firsthap+1,j)]=&rock.back();
	  miss[std::pair<int,int>(firsthap+2,j)]=&rock.back();
	  //
	  data[firsthap+3][j]=one;
	  rock.push_back(setrocker(data[firsthap+3]+j));
	  miss[std::pair<int,int>(firsthap+3,j)]=&rock.back();
	} else {
	  assert(d[firsthap+1][pos]==N);
	  data[firsthap+1][j]=one;
	  rock.push_back(setrocker(data[firsthap+1]+j));
	  miss[std::pair<int,int>(firsthap+1,j)]=&rock.back();
	  //
	  data[firsthap][j]=zero;
	  data[firsthap+2][j]=one;
	  data[firsthap+3][j]=zero;
	  rock.push_back(setrocker(data[firsthap]+j,data[firsthap+2]+j
				       ,data[firsthap+3]+j));
	  miss[std::pair<int,int>(firsthap,j)]=&rock.back();
	  miss[std::pair<int,int>(firsthap+2,j)]=&rock.back();
	  miss[std::pair<int,int>(firsthap+3,j)]=&rock.back();
	}
	break;
      case eithertrans:
	assert(d[firsthap+2][pos]==eithertrans
	       &&d[firsthap+1][pos]==N&&d[firsthap+3][pos]==N);
	data[firsthap][j]=one;
	data[firsthap+2][j]=zero;
	rock.push_back(setrocker(data[firsthap]+j,data[firsthap+2]+j));
	miss[std::pair<int,int>(firsthap,j)]=&rock.back();
	miss[std::pair<int,int>(firsthap+2,j)]=&rock.back();
	//
	data[firsthap+1][j]=one;
	rock.push_back(setrocker(data[firsthap+1]+j));
	miss[std::pair<int,int>(firsthap+1,j)]=&rock.back();
	//
	data[firsthap+3][j]=one;
	rock.push_back(setrocker(data[firsthap+3]+j));
	miss[std::pair<int,int>(firsthap+3,j)]=&rock.back();
	break;	    
      default:
	//look through the two pairs independently
	for (int k=0;k<4;k+=2) {
	  int start=firsthap+k;
	  if (d[start][pos]==hetpair) {
	    assert(d[start+1][pos]==hetpair);
	    data[start][j]=one;
	    data[start+1][j]=zero;
	    rock.push_back(setrocker(data[start]+j,data[start+1]+j));
	    miss[std::pair<int,int>(start,j)]=&rock.back();
	    miss[std::pair<int,int>(start+1,j)]=&rock.back();
	  } else { // can treat each individually
	    if (d[start][pos]==N) {
	      data[start][j]=one;
	      rock.push_back(setrocker(data[start]+j));
	      miss[std::pair<int,int>(start,j)]=&rock.back();
	    } else data[start][j]=d[start][pos];
	    if (d[start+1][pos]==N) {
	      data[start+1][j]=one;
	      rock.push_back(setrocker(data[start+1]+j));
	      miss[std::pair<int,int>(start+1,j)]=&rock.back();
	    } else data[start+1][j]=d[start+1][pos];			   
	  }
	}
      } 
    }
  }
}
std::ostream &operator<<(std::ostream &o, const triomissinghaplo &m)
{
  return m.print(o);
}



 
