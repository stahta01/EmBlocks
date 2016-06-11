/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */
/*
    This file is part of Em::Blocks.

    Copyright (c) 2011-2013 Em::Blocks

    Em::Blocks is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Em::Blocks is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Em::Blocks.  If not, see <http://www.gnu.org/licenses/>.

	@version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:
*/

#ifndef EDITOR_HOOKS_H
#define EDITOR_HOOKS_H

#include "settings.h"

class cbEditor;
class wxScintillaEvent;

/** Provides static functions to add hooks to the project loading/saving procedure. */
namespace EditorHooks
{
    /** Abstract base hook functor interface. */
    class DLLIMPORT HookFunctorBase
    {
        public:
            virtual ~HookFunctorBase(){}
            virtual void Call(cbEditor*, wxScintillaEvent&) const = 0;
    };

    /** Functor class for use as a project loading/saving hook.
      * Passed as the first parameter in RegisterHook() and
      * UnregisterHook().
      *
      * example:
      * EditorHooks::HookFunctorBase* myhook = new EditorHooks::HookFunctor<MyClass>(this, &MyClass::OnHookCalled);
      * int id = EditorHooks::RegisterHook(myhook);
      * ...
      * (and before your class' destruction - or earlier):
      * EditorHooks::UnregisterHook(id, true);
      *
      * Member functions used as hook callbacks must have the following signature:
      * void YourFunctionName(cbProject*, TiXmlElement*, bool)
      *
      * Use normal TinyXML procedures to work with the TiXmlElement* argument.
      * The isLoading argument is true if your hook is called when the project is being loaded,
      * and false when the project is saved.
      */
    template<class T> class HookFunctor : public HookFunctorBase
    {
        public:
            typedef void (T::*Func)(cbEditor*, wxScintillaEvent&);
            HookFunctor(T* obj, Func func)
                : m_pObj(obj),
                m_pFunc(func)
            {}
            virtual void Call(cbEditor* editor, wxScintillaEvent& event) const
            {
                if (m_pObj && m_pFunc)
                    (m_pObj->*m_pFunc)(editor, event);
            }
        protected:
            T* m_pObj;
            Func m_pFunc;
    };

    /** Register a project loading/saving hook.
      * @param functor The functor to use as a callback.
      * @return An ID. Use this to unregister your hook later.
      */
    extern DLLIMPORT int RegisterHook(HookFunctorBase* functor);
    /** Unregister a previously registered project loading/saving hook.
      * @param id The hook's ID. You should have the ID from when RegisterHook() was called.
      * @param deleteHook If true, the hook will be deleted (default). If not, it's
      * up to you to delete it.
      * @return The functor. If @c deleteHook was true, it always returns NULL.
      */
    extern DLLIMPORT HookFunctorBase* UnregisterHook(int id, bool deleteHook = true);
    /** Are there any hooks registered?
      * @return True if any hooks are registered, false if none.
      */
    extern DLLIMPORT bool HasRegisteredHooks();
    /** Call all registered hooks using the supplied parameters.
      * This is called by ProjectLoader.
      * @param editor The editor in question.
      * @param event Paremeter (wxScintilla event) to provide to the registered hook
      */
    extern DLLIMPORT void CallHooks(cbEditor* editor, wxScintillaEvent& event);
};

#endif // EDITOR_HOOKS_H
