/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/file.h>
#include <wx/thread.h>
#include <wx/stream.h>
#include <wx/stopwatch.h>

#include <settings.h>
#include <logmanager.h>
#include <globals.h>

#include "searchtree.h"

#include <deque>
#include <memory>

// Make sure already entered a critical section if TokensTree related!
extern wxCriticalSection s_TokensTreeCritical;

extern bool g_EnableDebugTrace;
extern const wxString g_DebugTraceFile;

#define CC_PARSER_PROFILE_TEST 0

#if CC_PARSER_PROFILE_TEST
#define CC_PROFILE_TIMER()                                                                          \
    static ProfileTimerData __ptd;                                                                  \
    static size_t __i = ProfileTimer::Registry(&__ptd, wxString(__PRETTY_FUNCTION__, wxConvUTF8));  \
    __ptd.m_CallTimes += __i;                                                                       \
    ProfileTimerHelper __profileTimerHelper(__ptd)
#define CC_PROFILE_TIMER_LOG() ProfileTimer::Log()
#else
#define CC_PROFILE_TIMER()
#define CC_PROFILE_TIMER_LOG()
#endif

class ProfileTimerData
{
public:
    ProfileTimerData() : m_CallTimes(0), m_Count(0)
    {
        m_StopWatch.Pause();
    }
    void Zero()
    {
        m_StopWatch.Start();
        m_StopWatch.Pause();
        m_CallTimes = 0;
        m_Count = 0;
    }
    wxStopWatch m_StopWatch;
    size_t m_CallTimes;
    size_t m_Count;
};

class ProfileTimerHelper
{
public:
    ProfileTimerHelper(ProfileTimerData& profileTimerData) : m_ProfileTimerData(profileTimerData)
    {
        if (m_ProfileTimerData.m_Count++ == 0)
            m_ProfileTimerData.m_StopWatch.Resume();
    }
    ~ProfileTimerHelper()
    {
        if (--m_ProfileTimerData.m_Count == 0)
            m_ProfileTimerData.m_StopWatch.Pause();
    }

private:
    ProfileTimerData& m_ProfileTimerData;
};

class ProfileTimer
{
public:
    static size_t Registry(ProfileTimerData* ptd, const wxString& funcName)
    {
        m_ProfileMap[ptd] = funcName;
        return 1;
    }

    static void Log()
    {
        for (ProfileMap::iterator it = m_ProfileMap.begin(); it != m_ProfileMap.end(); ++it)
        {
            const long totalTime = it->first->m_StopWatch.Time();
            wxString log;
            log.Printf(_T("\"%s\" used time is %ld minute(s), %ld.%03ld seconds; call times is %lu."),
                       it->second.wx_str(),
                       (totalTime / 60000),
                       (totalTime / 1000) % 60,
                       (totalTime % 1000),
                       static_cast<unsigned long>(it->first->m_CallTimes));
#ifndef CC_PARSER_TEST
            Manager::Get()->GetLogManager()->DebugLog(log);
#endif
            it->first->Zero();
        }
    }

private:
    typedef std::map<ProfileTimerData*, wxString> ProfileMap;
    static ProfileMap m_ProfileMap;
};

#ifndef CC_LOG_SYNC_SEND
    #define CC_LOG_SYNC_SEND 0
#endif

#ifdef CC_PARSER_TEST
    #undef CC_LOG_SYNC_SEND
    #define CC_LOG_SYNC_SEND 1
#endif

class CCLogger
{
public:
    static CCLogger* Get()
    {
        if (!s_Inst.get())
            s_Inst.reset(new CCLogger);
        return s_Inst.get();
    }

    void Init(wxEvtHandler* parent, int logId, int debugLogId)
    {
        m_Parent     = parent;
        m_LogId      = logId;
        m_debugLogId = debugLogId;
    }

    void Log(const wxString& msg)
    {
        wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, m_LogId);
        evt.SetString(msg);
#if CC_LOG_SYNC_SEND
        m_Parent->ProcessEvent(evt);
#else
        wxPostEvent(m_Parent, evt);
#endif
    }

    void DebugLog(const wxString& msg)
    {
        wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, m_debugLogId);
        evt.SetString(msg);
#if CC_LOG_SYNC_SEND
        m_Parent->ProcessEvent(evt);
#else
        wxPostEvent(m_Parent, evt);
#endif
    }

protected:
    CCLogger() : m_Parent(_nullptr), m_LogId(0) {}
    virtual ~CCLogger() {}
    CCLogger(const CCLogger&) {}
    CCLogger& operator= (const CCLogger&) { return *this; }
    friend class std::auto_ptr<CCLogger>;
    static std::auto_ptr<CCLogger> s_Inst;

private:
    wxEvtHandler* m_Parent;
    int m_LogId;
    int m_debugLogId;
};


#ifdef CC_ENABLE_LOCKER_TRACK
    #define TRACK_THREAD_LOCKER(NAME)                                                               \
        CCLockerTrack NAME##Track(wxString(#NAME, wxConvUTF8),                                      \
                                  wxString(__FUNCTION__, wxConvUTF8),                               \
                                  wxString(__FILE__, wxConvUTF8),                                   \
                                  __LINE__,                                                         \
                                  wxIsMainThread())
    #define THREAD_LOCKER_SUCCESS(NAME)                                                             \
        CCLogger::Get()->DebugLog(F(_T("%s.Success() : %s(), %s, %d"),                              \
                                    wxString(#NAME, wxConvUTF8).wx_str(),                           \
                                    wxString(__FUNCTION__, wxConvUTF8).wx_str(),                    \
                                    wxString(__FILE__, wxConvUTF8).wx_str(),                        \
                                    __LINE__))
#else
    #define TRACK_THREAD_LOCKER(NAME)
    #define THREAD_LOCKER_SUCCESS(NAME)
#endif

class CCLockerTrack
{
public:
    CCLockerTrack(const wxString& locker, const wxString& func, const wxString& file, int line,
                  bool mainThread) :
        m_LockerName(locker),
        m_FuncName(func),
        m_FileName(file),
        m_Line(line),
        m_MainThread(mainThread)
    {
        CCLogger::Get()->DebugLog(F(_T("%s.Lock() : %s(), %d, %s, %d"), m_LockerName.wx_str(),
                                    m_FuncName.wx_str(), m_MainThread, m_FileName.wx_str(), m_Line));
    }
    ~CCLockerTrack()
    {
        CCLogger::Get()->DebugLog(F(_T("%s.UnLock() : %s(), %d, %s, %d"), m_LockerName.wx_str(),
                                    m_FuncName.wx_str(), m_MainThread, m_FileName.wx_str(), m_Line));
    }

private:
    wxString m_LockerName;
    wxString m_FuncName;
    wxString m_FileName;
    int      m_Line;
    bool     m_MainThread;
};

class Token;
class TokensTree;

enum FileParsingStatus
{
    fpsNotParsed = 0,
    fpsAssigned,
    fpsBeingParsed,
    fpsDone
};

enum CCFileType
{
    ccftHeader,
    ccftSource,
    ccftOther
};

CCFileType CCFileTypeOf(const wxString& filename);

WX_DEFINE_ARRAY(Token*, TokensArray);

typedef std::vector<Token*> TokenList;

typedef std::deque<int>                                          TokenIdxList;
typedef std::set< int, std::less<int> >                          TokenIdxSet;
typedef SearchTree<TokenIdxSet>                                  TokenSearchTree;
typedef BasicSearchTree                                          TokenFilenamesMap;
typedef std::map< size_t, TokenIdxSet,       std::less<size_t> > TokenFilesMap;
typedef std::map< size_t, FileParsingStatus, std::less<size_t> > TokenFilesStatus;
typedef std::set< size_t, std::less<size_t> >                    TokenFilesSet;

enum TokenScope
{
    tsUndefined = 0,
    tsPrivate,
    tsProtected,
    tsPublic
};

enum TokenKind
{
    // changed in order to reflect the priority
    tkNamespace     = 0x0001,
    tkClass         = 0x0002,
    tkEnum          = 0x0004,
    tkTypedef       = 0x0008, // typedefs are stored as classes inheriting from the typedef'd type (taking advantage of existing inheritance code)
    tkConstructor   = 0x0010,
    tkDestructor    = 0x0020,
    tkFunction      = 0x0040,
    tkVariable      = 0x0080,
    tkEnumerator    = 0x0100,
    tkPreprocessor  = 0x0200,
    tkMacro         = 0x0400,

    // convenient masks
    tkAnyContainer  = tkClass    | tkNamespace   | tkTypedef,
    tkAnyFunction   = tkFunction | tkConstructor | tkDestructor,

    // undefined or just "all"
    tkUndefined     = 0xFFFF
};


class Token
{
friend class TokensTree;
public:
    Token(const wxString& name, unsigned int file, unsigned int line, size_t ticket);
    ~Token();

    bool AddChild(int childIdx);
    bool DeleteAllChildren();
    bool HasChildren() { return !m_Children.empty(); }
    wxString GetNamespace() const;
    bool InheritsFrom(int idx) const;
    wxString DisplayName() const;
    wxString GetTokenKindString() const;
    wxString GetTokenScopeString() const;
    wxString GetFilename() const;
    wxString GetImplFilename() const;
    wxString GetFormattedArgs() const; // remove all '\n'
    wxString GetStrippedArgs() const; // remove all default value
    size_t GetTicket() const { return m_Ticket; }
    bool MatchesFiles(const TokenFilesSet& files);

    bool SerializeIn(wxInputStream* f);
    bool SerializeOut(wxOutputStream* f);

    TokensTree* GetTree() { return m_TokensTree; }
    bool IsValidAncestor(const wxString& ancestor);

    wxString                     m_Type;       // this is the return value (if any): e.g. const wxString&
    wxString                     m_ActualType; // this is what the parser believes is the actual return value: e.g. wxString
    wxString                     m_Name;
    wxString                     m_Args;
    wxString                     m_BaseArgs;
    wxString                     m_AncestorsString; // all ancestors comma-separated list
    wxString                     m_TemplateArgument;
    unsigned int                 m_FileIdx;
    unsigned int                 m_Line;
    unsigned int                 m_ImplFileIdx;
    unsigned int                 m_ImplLine;      // where the token was met
    unsigned int                 m_ImplLineStart; // if token is impl, opening brace line
    unsigned int                 m_ImplLineEnd;   // if token is impl, closing brace line
    TokenScope                   m_Scope;
    TokenKind                    m_TokenKind;
    bool                         m_IsOperator;
    bool                         m_IsLocal;       // found in a local file?
    bool                         m_IsTemp;        // local variable
    bool                         m_IsPredefine;   // Compiler predefine
    bool                         m_IsConst;       // the member method is const (yes/no)

    int                          m_Index;         // current index in the tree
    int                          m_ParentIndex;
    TokenIdxSet                  m_Children;
    TokenIdxSet                  m_Ancestors;
    TokenIdxSet                  m_DirectAncestors;
    TokenIdxSet                  m_Descendants;

    wxArrayString                m_Aliases; // used for namespace aliases

    wxArrayString                m_TemplateType;
    std::map<wxString, wxString> m_TemplateMap;

    wxString                     m_TemplateAlias; // alias for templates, e.g. template T1 T2;
    void*                        m_UserData; // custom user-data (the classbrowser expects it to be a pointer to a cbProject)

protected:
    TokensTree*                  m_TokensTree;
    size_t                       m_Ticket;
};

class TokensTree
{
public:
    TokensTree();
    virtual ~TokensTree();

    inline void Clear()               { clear(); }

    // STL compatibility functions
    void          clear();
    inline Token* operator[](int idx) { return GetTokenAt(idx); }
    inline Token* at(int idx)         { return GetTokenAt(idx); }
    inline const Token * at(int idx) const { return GetTokenAt(idx); }
    size_t        size();
    size_t        realsize();
    inline bool   empty()             { return size()==0; }
    int           insert(Token* newToken);
    int           insert(int loc, Token* newToken);
    int           erase(int loc);
    void          erase(Token* oldToken);

    // Token specific functions
    void   RecalcFreeList();
    void   RecalcInheritanceChain(Token* token);
    int    TokenExists(const wxString& name, int parent, short int kindMask);
    int    TokenExists(const wxString& name, const wxString& baseArgs, int parent, TokenKind kind);
    size_t FindMatches(const wxString& s, TokenIdxSet& result, bool caseSensitive, bool is_prefix, short int kindMask = tkUndefined);
    size_t FindTokensInFile(const wxString& file, TokenIdxSet& result, short int kindMask);
    void   RemoveFile(const wxString& filename);
    void   RemoveFile(int fileIndex);

    // Parsing related functions
    size_t         GetFileIndex(const wxString& filename);
    const wxString GetFilename(size_t idx) const;
    size_t         ReserveFileForParsing(const wxString& filename, bool preliminary = false);
    void           FlagFileForReparsing(const wxString& filename);
    void           FlagFileAsParsed(const wxString& filename);
    bool           IsFileParsed(const wxString& filename);

    void MarkFileTokensAsLocal(const wxString& filename, bool local = true, void* userData = 0);
    void MarkFileTokensAsLocal(size_t file, bool local = true, void* userData = 0);

    TokenList         m_Tokens;            /** Contains the pointers to all the tokens */
    TokenSearchTree   m_Tree;              /** Tree containing the indexes to the tokens (the indexes will be used on m_Tokens) */

    TokenFilenamesMap m_FilenamesMap;      /** Map: filenames    -> file indexes */
    TokenFilesMap     m_FilesMap;          /** Map: file indexes -> sets of TokenIndexes */
    TokenFilesSet     m_FilesToBeReparsed; /** Set: file indexes */
    TokenIdxList      m_FreeTokens;        /** List of all the deleted (and available) tokens */

    /** List of tokens belonging to the global namespace */
    TokenIdxSet       m_TopNameSpaces;
    TokenIdxSet       m_GlobalNameSpace;

    TokenFilesStatus  m_FilesStatus;       /** Parse status for each file */
    bool              m_Modified;
    size_t            m_StructUnionUnnamedCount;
    size_t            m_EnumUnnamedCount;
    size_t            m_TokenTicketCount;

protected:
    Token* GetTokenAt(int idx);
    Token const * GetTokenAt(int idx) const;
    int AddToken(Token* newToken, int fileIndex);

    void RemoveToken(int idx);
    void RemoveToken(Token* oldToken);

    int  AddTokenToList(Token* newToken, int forceidx);
    void RemoveTokenFromList(int idx);

    void RecalcFullInheritance(int parentIdx, TokenIdxSet& result); // called by RecalcData

    /** Check all the children belong this token should be remove
      * @param token the checked token pointer
      * @param fileIndex file index the token belongs to
      * @return if true, we can safely remove the token
      */
    bool CheckChildRemove(Token * token, int fileIndex);
};

inline void SaveIntToFile(wxOutputStream* f, int i)
{
    /* This used to be done as
        f->Write(&i, sizeof(int));
    which is incorrect because it assumes a consistant byte order
    and a constant int size */

    unsigned int const j = i; // rshifts aren't well-defined for negatives
    wxChar c[4] = { (wxChar) (j>> 0&0xFF),
                    (wxChar) (j>> 8&0xFF),
                    (wxChar) (j>>16&0xFF),
                    (wxChar) (j>>24&0xFF) };
    f->Write( c, 4 );
}

inline bool LoadIntFromFile(wxInputStream* f, int* i)
{
//    See SaveIntToFile
//    return f->Read(i, sizeof(int)) == sizeof(int);

    wxChar c[4];
    if ( f->Read( c, 4 ).LastRead() != 4 ) return false;
    *i = ( c[0]<<0 | c[1]<<8 | c[2]<<16 | c[3]<<24 );
    return true;
}

inline void SaveStringToFile(wxOutputStream* f, const wxString& str)
{
    const wxWX2MBbuf psz = str.mb_str(wxConvUTF8);
    // TODO (MortenMacFly#5#): Can we safely use strlen here?
    int size = psz ? strlen(psz) : 0;
    if (size >= 32767)
        size = 32767;
    SaveIntToFile(f, size);
    if(size)
        f->Write(psz, size);
}

inline bool LoadStringFromFile(wxInputStream* f, wxString& str)
{
    int size;
    if (!LoadIntFromFile(f, &size))
        return false;
    bool ok = true;
    if (size > 0 && size <= 32767)
    {
        wxChar buf[32768];
        ok = f->Read(buf, size).LastRead() == (size_t)size;
        buf[size] = '\0';
        str = wxString(buf, wxConvUTF8);
    }
    else // doesn't fit in our buffer, but still we have to skip it
    {
        str.Empty();
        size = size & 0xFFFFFF; // Can't get any longer than that
        f->SeekI(size, wxFromCurrent);
    }
    return ok;
}

#endif // TOKEN_H