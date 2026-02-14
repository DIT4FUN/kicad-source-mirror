/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2026 Jon Evans <jon@craftyjon.com>
 * Copyright The KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <widgets/footprint_wizard_properties_panel.h>

#include <footprint_wizard.h>
#include <footprint_wizard_frame.h>
#include <properties/pg_editors.h>
#include <properties/pg_properties.h>
#include <widgets/ui_common.h>

#include <wx/stattext.h>


FOOTPRINT_WIZARD_PROPERTIES_PANEL::FOOTPRINT_WIZARD_PROPERTIES_PANEL( wxWindow* aParent,
                                                                      FOOTPRINT_WIZARD_FRAME* aFrame ) :
        PROPERTIES_PANEL( aParent, aFrame ),
        m_frame( aFrame ),
        m_wizard( nullptr ),
        m_unitEditorInstance( nullptr ),
        m_checkboxEditorInstance( nullptr ),
        m_ratioEditorInstance( nullptr )
{
    m_caption->SetLabel( _( "Parameters" ) );

    wxASSERT( wxPGGlobalVars );

    wxString editorKey = PG_UNIT_EDITOR::BuildEditorName( m_frame );
    auto it = wxPGGlobalVars->m_mapEditorClasses.find( editorKey );

    if( it != wxPGGlobalVars->m_mapEditorClasses.end() )
    {
        m_unitEditorInstance = static_cast<PG_UNIT_EDITOR*>( it->second );
        m_unitEditorInstance->UpdateFrame( m_frame );
    }
    else
    {
        PG_UNIT_EDITOR* new_editor = new PG_UNIT_EDITOR( m_frame );
        m_unitEditorInstance = static_cast<PG_UNIT_EDITOR*>( wxPropertyGrid::RegisterEditorClass( new_editor ) );
    }

    it = wxPGGlobalVars->m_mapEditorClasses.find( PG_CHECKBOX_EDITOR::EDITOR_NAME );

    if( it == wxPGGlobalVars->m_mapEditorClasses.end() )
    {
        PG_CHECKBOX_EDITOR* cbEditor = new PG_CHECKBOX_EDITOR();
        m_checkboxEditorInstance = static_cast<PG_CHECKBOX_EDITOR*>( wxPropertyGrid::RegisterEditorClass( cbEditor ) );
    }
    else
    {
        m_checkboxEditorInstance = static_cast<PG_CHECKBOX_EDITOR*>( it->second );
    }

    it = wxPGGlobalVars->m_mapEditorClasses.find( PG_RATIO_EDITOR::EDITOR_NAME );

    if( it == wxPGGlobalVars->m_mapEditorClasses.end() )
    {
        PG_RATIO_EDITOR* ratioEditor = new PG_RATIO_EDITOR();
        m_ratioEditorInstance = static_cast<PG_RATIO_EDITOR*>( wxPropertyGrid::RegisterEditorClass( ratioEditor ) );
    }
    else
    {
        m_ratioEditorInstance = static_cast<PG_RATIO_EDITOR*>( it->second );
    }
}


FOOTPRINT_WIZARD_PROPERTIES_PANEL::~FOOTPRINT_WIZARD_PROPERTIES_PANEL()
{
    if( m_unitEditorInstance )
        m_unitEditorInstance->UpdateFrame( nullptr );
}


void FOOTPRINT_WIZARD_PROPERTIES_PANEL::UpdateData()
{
}


wxPGProperty* FOOTPRINT_WIZARD_PROPERTIES_PANEL::createPGProperty( const PROPERTY_BASE* ) const
{
    return new wxPropertyCategory();
}


void FOOTPRINT_WIZARD_PROPERTIES_PANEL::RebuildParameters( FOOTPRINT_WIZARD* aWizard )
{
    // TODO
}


void FOOTPRINT_WIZARD_PROPERTIES_PANEL::valueChanged( wxPropertyGridEvent& aEvent )
{
    // TODO
}
