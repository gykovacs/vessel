#include <openipLL/ComponentLabeling.h>

#include <openipDS/Set.h>
#include <openipDS/Vector.h>
#include <openipDS/Stopper.h>

namespace openip
{
    class EquivalenceClasses
    {
    public:
        EquivalenceClasses();
        
        EquivalenceClasses(const EquivalenceClasses& e);
        
        ~EquivalenceClasses();
        
        int getClass(int);
        
        void insert(int, int);
        
        void reduce();
        
        Vector<Set<int> > classes;
        Vector<int> map;
    };
    
    EquivalenceClasses::EquivalenceClasses()
    {
        map.clear();
        classes.clear();
    }
        
    EquivalenceClasses::EquivalenceClasses(const EquivalenceClasses& /*e*/)
    {
    }

    EquivalenceClasses::~EquivalenceClasses()
    {
    }

    int EquivalenceClasses::getClass(int element)
    {
        return map(element);
    }

    void EquivalenceClasses::insert(int a, int b)
    {
        if ( map.size() <= (unsigned int)(a) )
            map.resize(a + 1, -1);
        if ( map.size() <= (unsigned int)(b) )
            map.resize(b + 1, -1);
        
        if ( map(a) == -1 && map(b) == -1 )
        {
            classes.push_back( (Set<int>()) );
            map(a)= classes.size() - 1;
            map(b)= classes.size() - 1;
            //classes(map(a)).insert(a);
            //classes(map(b)).insert(b);
	    classes(map(a)).push_back(a);
	    classes(map(b)).push_back(b);
        }
        else if ( map(a) == map(b) )
        {
            return;
        }
        else if ( map(a) >= 0 && map(b) == -1 )
        {
            //classes(map(a)).insert(b);
	    classes(map(a)).push_back(b);
            map(b)= map(a);
        }
        else if ( map(b) >= 0 && map(a) == -1 )
        {
            //classes(map(b)).insert(a);
	    classes(map(b)).push_back(a);
            map(a)= map(b);
        }
        else if ( map(a) != map(b) && map(a) >= 0 && map(b) >= 0 )
        {
            vector<int> tmp;
            for ( Set<int>::iterator it= classes(map(b)).begin(); it != classes(map(b)).end(); ++it )
            {
                if ( *it != b )
                {
                //classes(map(a)).insert(*it);
		classes(map(a)).push_back(*it);
                tmp.push_back(*it);
                }
            }
            sort(classes(map(a)).begin(), classes(map(a)).end());
	    Vector<int>::iterator it= unique(classes(map(a)).begin(), classes(map(a)).end());
	    classes(map(a)).resize(std::distance(classes(map(a)).begin(), it));
            for ( unsigned int i= 0; i < tmp.size(); ++i )
            {
                map(tmp[i])= map(a);
            }
            classes(map(b)).clear();
            classes(map(a)).insert(b);
            map(b)= map(a);
        }
    }
    
    void EquivalenceClasses::reduce()
    {
        //tprintf("reduce %d\n", classes.size());
        int start= 0, end= classes.size() - 1;
        
        while ( start < end )
        {
            while ( classes(start).size() != 0 && start < end )
                ++start;
            while ( classes(end).size() == 0 && start < end )
                --end;
            if ( start < end )
            {
	        //delete classes(start);
                classes(start)= classes(end);
                classes.erase(classes.begin() + end);
                --end;
            }
        }

        unsigned int i;
        for ( i= 0; i < classes.size(); ++i )
            if ( classes(i).size() == 0 )
                break;
        classes.resize(i);
            
        for ( unsigned int i= 0; i < classes.size(); ++i )
        {
            for ( Set<int>::iterator it= classes(i).begin(); it != classes(i).end(); ++it )
            {
                map(*it)= i;
            }
        }
    }
    
    ComponentLabeling::ComponentLabeling(int neighborhood)
    : Transform2<unsigned char, int>()
    {
      this->neighborhood= neighborhood;
    }

    ComponentLabeling::ComponentLabeling(const ComponentLabeling& c)
    : Transform2<unsigned char, int>(c)
    {
    }

    ComponentLabeling::~ComponentLabeling()
    {
    }

    void ComponentLabeling::apply(Image<unsigned char>& input, Image<int>& output, Image<unsigned char>* )
    {
        output= 0;

        int label= 1;
        int a, b, c, d, nn= 0;
        EquivalenceClasses eqv;
        eqv.insert(label, label);
        
        for ( int i= 0; i < input.rows; ++i )
            for ( int j= 0; j < input.columns; ++j )
                if ( input(i,j) > 0 )
                {
		  if ( neighborhood == 2 )
		  {
		    if ( j-1 >= 0 )
		      a= output(i,j-1);
		    else
		      a= 0;
		    if ( i-1 >= 0 && j-1 >= 0 )
		      b= output(i-1, j-1);
		    else
		      b= 0;
		    if ( i-1 >= 0 )
		      c= output(i-1, j);
		    else
		      c= 0;
		    if ( i-1 >= 0 && j + 1 < input.columns )
		      d= output(i-1, j+1);
		    else
		      d= 0;
                    
                    if ( a == 0 && b == 0 && c == 0 && d == 0 )
                    {
                        output(i,j)= label++;
                        eqv.insert(label - 1, label - 1);
                    }
                    else
                    {
                        if ( a != 0 )
                            nn= a;
                        if ( b != 0 )
                            nn= b;
                        if ( c != 0 )
                            nn= c;
                        if ( d != 0 )
                            nn= d;
                        
                        if ( a != 0 )
                            eqv.insert(nn,a);
                        if ( b != 0 )
                            eqv.insert(nn,b);
                        if ( c != 0 )
                            eqv.insert(nn,c);
                        if ( d != 0 )
                            eqv.insert(nn,d);
                        
                        output(i,j)= nn;
                    }
		  }
		  else if ( neighborhood == 1 )
		  {
		    if ( j-1 >= 0 )
		      a= output(i,j-1);
		    else
		      a= 0;
		    if ( i-1 >= 0 )
		      c= output(i-1, j);
		    else
		      c= 0;
                    
                    if ( a == 0 && c == 0)
                    {
                        output(i,j)= label++;
                        eqv.insert(label - 1, label - 1);
                    }
                    else
                    {
                        if ( a != 0 )
                            nn= a;
                        if ( c != 0 )
                            nn= c;
                        
                        if ( a != 0 )
                            eqv.insert(nn,a);
                        if ( c != 0 )
                            eqv.insert(nn,c);
                        
                        output(i,j)= nn;
                    }
		  }
                }
        
        
        eqv.reduce();
        for ( unsigned int i= 0; i < output.n; ++i )
            if ( output(i) > 0 )
                output(i)= eqv.map(output(i))+1;
	
	/*for ( int i= 0; i < eqv.classes.size(); ++i )
	  delete eqv.classes[i];*/
    }
    
    ExtractRegions::ExtractRegions(int neighborhood)
    {
      this->neighborhood= neighborhood;
    }
        
    ExtractRegions::ExtractRegions(const ExtractRegions& /*e*/)
    {
    }

    ExtractRegions::~ExtractRegions()
    {
    }

    void ExtractRegions::apply(Image<unsigned char>& input, Vector<Region2>& result, bool foreground)
    {
	if ( input.getMax() == 0 )
	{
	  result.clear();
	  return;
	}
	
        ComponentLabeling cl(neighborhood);
        Image<int> tmp;
        tmp.resizeImage(input);
        
	//tprintf("cl\n");
        cl.apply(input, tmp);
	//tprintf("cl finished\n");

	//printf("m"); fflush(stdout);
        int min, max;
        tmp.getMinMax(min, max);
        
        result.resize(max - min + 1);

	//printf("n %d,%d,%d", result.size(), max, min); fflush(stdout);
        for ( unsigned int i= 0; i < result.size(); ++i )
        {
            result[i].stride= input.columns;
            result[i].clear();
        }
        //printf("o"); fflush(stdout);
        for ( unsigned int i= 0; i < input.n; ++i )
        {
            result[tmp(i)].push_back(i);
        }
        //printf("p"); fflush(stdout);
        if ( foreground == true )
	{
          result.erase(result.begin());
	}
	    
    }

    RecursiveComponentLabeling3::RecursiveComponentLabeling3()
        : Transform3<unsigned char, int>()
    {
    }

    RecursiveComponentLabeling3::RecursiveComponentLabeling3(const RecursiveComponentLabeling3& r)
        : Transform3<unsigned char, int>(r)
    {
    }

    RecursiveComponentLabeling3::~RecursiveComponentLabeling3()
    {
    }

    void RecursiveComponentLabeling3::apply(Volume<unsigned char>& input, Volume<int>& output, Volume<unsigned char>* roi)
    {
        output= 0;
        Volume<unsigned char> mask;
        mask.resizeVolume(input);
	Volume<unsigned char> tmp;
        tmp.resizeVolume(input);
        mask= 0;
	tmp= input;

	if ( roi )
	  for ( unsigned int i= 0; i < roi->n; ++i )
	    if ( !(*roi)(i) )
	      tmp(i)= 0;
	
        int label= 1;
	tprintf("start recursive section\n");
        for ( unsigned int i= 0; i < input.slices; ++i )
            for ( unsigned int j= 0; j < input.rows; ++j )
                for ( unsigned int k= 0; k < input.columns; ++k )
                {
                    if ( tmp(i,j,k) > 0 && mask(i,j,k) == 0 )
                    {
                        slices.push(i);
                        rows.push(j);
                        columns.push(k);
                        labeling(tmp, mask, output, label);
                        ++label;
                    }
                }
        tprintf("number of components: %d\n", label); fflush(stdout);
    }

    void RecursiveComponentLabeling3::apply(Volume<unsigned char>& input, Volume<int>& output, Region3& roi)
    {
        output= 0;
        Volume<unsigned char> mask;
	Volume<unsigned char> tmp;
        mask.resizeVolume(input);
	tmp.resizeVolume(input);
        mask= 0;

	tmp= input;
	for ( unsigned int i= 0; i < roi.size(); ++i )
	  mask(roi(i))= 1;
	for ( unsigned int i= 0; i < mask.n; ++i )
	  if ( !mask(i) )
	    tmp(i)= 0;
	mask= 0;
	
        int label= 1;
        for ( unsigned int i= 0; i < input.slices; ++i )
            for ( unsigned int j= 0; j < input.rows; ++j )
                for ( unsigned int k= 0; k < input.columns; ++k )
                {
                    if ( tmp(i,j,k) > 0 && mask(i,j,k) == 0 )
                    {
                        slices.push(i);
                        rows.push(j);
                        columns.push(k);
                        labeling(tmp, mask, output, label);
                        ++label;
                    }
                }
        //printf("number of components: %d\n", label); fflush(stdout);
    }
    
    void RecursiveComponentLabeling3::labeling(Volume<unsigned char>& input, Volume<unsigned char>& mask, Volume<int>& output, int label)
    {
        while ( ! slices.empty() )
        {
            unsigned int s= slices.front();
            unsigned int r= rows.front();
            unsigned int c= columns.front();
            slices.pop();
            rows.pop();
            columns.pop();

            if ( input(s, r, c) > 0 && mask(s, r, c) == 0 )
            {
                mask(s, r, c)= 1;
                output(s, r, c)= label;
                if ( s > 0 ){ slices.push(s-1); rows.push(r); columns.push(c); }
                if ( s > 0 && c > 0 ){ slices.push(s-1); rows.push(r); columns.push(c-1); }
                if ( s > 0 && c < input.columns-1 ){ slices.push(s-1); rows.push(r); columns.push(c+1); }
                if ( s > 0 && r > 0 ){ slices.push(s-1); rows.push(r-1); columns.push(c); }
                if ( s > 0 && r > 0 && c < input.columns-1 ){ slices.push(s-1); rows.push(r-1); columns.push(c+1); }
                if ( s > 0 && r > 0 && c > 0 ){ slices.push(s-1); rows.push(r-1); columns.push(c-1); }
                if ( s > 0 && r < input.rows-1 ){ slices.push(s-1); rows.push(r+1); columns.push(c); }
                if ( s > 0 && r < input.rows-1 && c > 0){ slices.push(s-1); rows.push(r+1); columns.push(c-1); }
                if ( s > 0 && r < input.rows-1 && c < input.columns-1){ slices.push(s-1); rows.push(r+1); columns.push(c+1); }
                if ( r > 0 && c > 0 ) { slices.push(s); rows.push(r-1); columns.push(c-1); }
                if ( r > 0 ) { slices.push(s); rows.push(r-1); columns.push(c); }
                if ( r > 0 && c < input.columns-1 ) { slices.push(s); rows.push(r-1); columns.push(c+1); }
                if ( c > 0 ) { slices.push(s); rows.push(r); columns.push(c-1); }
                { slices.push(s); rows.push(r); columns.push(c); }
                if ( c < input.columns-1 ) { slices.push(s); rows.push(r); columns.push(c+1); }
                if ( r < input.rows-1 && c > 0 ) { slices.push(s); rows.push(r+1); columns.push(c-1); }
                if ( r < input.rows-1 ) { slices.push(s); rows.push(r+1); columns.push(c); }
                if ( r < input.rows-1 && c < input.columns-1 ) { slices.push(s); rows.push(r+1); columns.push(c+1); }
                if ( s < input.slices-1 && r > 0 && c > 0 ){ slices.push(s+1); rows.push(r-1); columns.push(c-1); }
                if ( s < input.slices-1 && r > 0 ){ slices.push(s+1); rows.push(r-1); columns.push(c); }
                if ( s < input.slices-1 && r > 0 && c < input.columns-1 ){ slices.push(s+1); rows.push(r-1); columns.push(c+1); }
                if ( s < input.slices-1 && c > 0 ){ slices.push(s+1); rows.push(r); columns.push(c-1); }
                if ( s < input.slices-1 ){ slices.push(s+1); rows.push(r); columns.push(c); }
                if ( s < input.slices-1 && c < input.columns-1 ){ slices.push(s+1); rows.push(r); columns.push(c+1); }
                if ( s < input.slices-1 && r < input.rows-1 && c > 0 ){ slices.push(s+1); rows.push(r+1); columns.push(c-1); }
                if ( s < input.slices-1 && r < input.rows-1 ){ slices.push(s+1); rows.push(r+1); columns.push(c); }
                if ( s < input.slices-1 && r < input.rows-1 && c < input.columns-1){ slices.push(s+1); rows.push(r+1); columns.push(c+1); }
            }
        }
    }

    ExtractRegions3::ExtractRegions3()
    {
    }

    ExtractRegions3::ExtractRegions3(const ExtractRegions3& /*e*/)
    {
    }

    ExtractRegions3::~ExtractRegions3()
    {
    }

    void ExtractRegions3::apply(Volume<unsigned char>& input, Region3Set& result, Volume<unsigned char>* roi, bool /*foreground*/)
    {
        Volume<int> output;
        output.resizeVolume(input);
        RecursiveComponentLabeling3 rcl;

	tprintf("recursive component labeling...\n");
        rcl.apply(input, output, roi);
	tprintf("recursive labeling finished\n");

        int label= output.getMax();

        result.resize(label);

        for ( unsigned int i= 0; i < output.n; ++i )
            if ( output(i) > 0 )
                result(output(i)-1).push_back(i);

        for ( unsigned int i= 0; i < result.size(); ++i )
        {
            result(i).stride= input.columns;
            result(i).slicePitch= input.sliceSize;
        }
    }
    
    void ExtractRegions3::apply(Volume<unsigned char>& input, Region3Set& result, Region3& roi, bool /*foreground*/)
    {
        Volume<int> output;
        output.resizeVolume(input);
        RecursiveComponentLabeling3 rcl;

        rcl.apply(input, output, roi);

        int label= output.getMax();

        result.resize(label);

        for ( unsigned int i= 0; i < output.n; ++i )
            if ( output(i) > 0 )
                result(output(i)-1).push_back(i);

        for ( unsigned int i= 0; i < result.size(); ++i )
        {
            result(i).stride= input.columns;
            result(i).slicePitch= input.sliceSize;
        }
    }
}
