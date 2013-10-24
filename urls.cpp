#include "urls.h"
static const string forbidden[] = 
{
	".tar",".gz",".tgz",".zip",".Z",".rpm",".deb"
		,".ps",".dvi",".pdf"
		," .png",".jpg",".jpeg",".bmp",".smi",".tiff",".gif"
		,".mov",".avi"," .mpeg", ".mpg" ,".mp3" ,".qt" ,".wav" ,".ram" ,".rm"
		,".jar", ".java", ".class", ".diff"
		,".doc", ".xls", ".ppt" ,".mdb" ,".rtf", ".exe", ".pps", ".so", ".psd"
};
typedef map < string, size_t > URLDB;
static URLDB mDB;

bool url_exist(string url)
{
	if ( mDB.find(url) == mDB.end() )
		return false;
	else
		return true;
}

bool url_add(string url)
{
	return mDB.insert( URLDB::value_type( url, 1) ).second;
}
bool url_filter ( string url )
{
        size_t pos =  url.rfind( "." );
	        
        if ( pos == url.npos )
                return false;
       
        string subfix = url.substr( pos, -1 );
			        
        for ( size_t i = 0; i < sizeof(forbidden) / sizeof(forbidden[0]); i++ )
        {
                if ( subfix == forbidden[i] )
                        return true;
        }

        return false;
}
