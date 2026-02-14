/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012 Miguel Angel Ajo Pelayo, miguelangel@nbee.es
 * Copyright (C) 2018 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright The KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <pcb_edit_frame.h>
#include <board.h>
#include <footprint.h>
#include <pcbnew_id.h>
#include <wildcards_and_files_ext.h>
#include <dialogs/dialog_footprint_wizard_list.h>
#include <wx/msgdlg.h>
#include <tool/tool_manager.h>
#include "footprint_wizard_frame.h"


/* Displays the name of the current opened library in the caption */
void FOOTPRINT_WIZARD_FRAME::DisplayWizardInfos()
{
    wxString msg;

    msg = _( "Footprint Wizard" );
    msg << wxT( " [" );

    if( !m_wizardName.IsEmpty() )
        msg << m_wizardName;
    else
        msg += _( "no wizard selected" );

    msg << wxT( "]" );

    SetTitle( msg );
}


void FOOTPRINT_WIZARD_FRAME::RegenerateFootprint()
{
    FOOTPRINT_WIZARD* footprintWizard = GetMyWizard();

    if( !footprintWizard )
        return;

    m_toolManager->ResetTools( TOOL_BASE::MODEL_RELOAD );

    // Delete the current footprint
    GetBoard()->DeleteAllFootprints();

    // Creates the footprint
    // TODO(JE)
#if 0
    wxString   msg;
    FOOTPRINT* footprint = footprintWizard->GetFootprint( &msg );
    DisplayBuildMessage( msg );

    if( footprint )
    {
        //  Add the object to board
        GetBoard()->Add( footprint, ADD_MODE::APPEND );
        footprint->SetPosition( VECTOR2I( 0, 0 ) );
    }
#endif

    updateView();
    GetCanvas()->Refresh();
}


void FOOTPRINT_WIZARD_FRAME::DisplayBuildMessage( wxString& aMessage )
{
    m_buildMessageBox->SetValue( aMessage );
}


FOOTPRINT_WIZARD* FOOTPRINT_WIZARD_FRAME::GetMyWizard()
{
    if( m_wizardName.Length() == 0 )
        return nullptr;

    // TODO(JE)
    return nullptr;
}


FOOTPRINT* FOOTPRINT_WIZARD_FRAME::GetBuiltFootprint()
{
    // TODO(JE)

    return nullptr;
}


void FOOTPRINT_WIZARD_FRAME::SelectFootprintWizard()
{
    DIALOG_FOOTPRINT_WIZARD_LIST wizardSelector( this );

    if( wizardSelector.ShowModal() != wxID_OK )
        return;

    FOOTPRINT_WIZARD* footprintWizard = wizardSelector.GetWizard();

    if( footprintWizard )
    {
        // TODO(JE)
#if 0
        m_wizardName = footprintWizard->GetName();
        m_wizardDescription = footprintWizard->GetDescription();

        footprintWizard->ResetParameters();
#endif
    }
    else
    {
        m_wizardName.Empty();
        m_wizardDescription.Empty();
    }

    RegenerateFootprint();
    Zoom_Automatique( false );
    DisplayWizardInfos();
    ReCreateParameterList();
}


void FOOTPRINT_WIZARD_FRAME::SelectCurrentWizard( wxCommandEvent& aDummy )
{
    SelectFootprintWizard();
    updateView();
}

void FOOTPRINT_WIZARD_FRAME::DefaultParameters()
{
    FOOTPRINT_WIZARD* footprintWizard = GetMyWizard();

    if ( footprintWizard == nullptr )
        return;

    // TODO(JE)
#if 0
    footprintWizard->ResetParameters();
#endif

    // Reload
    ReCreateParameterList();
    RegenerateFootprint();
    DisplayWizardInfos();
}


void FOOTPRINT_WIZARD_FRAME::RebuildWizardParameters()
{
    ReCreateParameterList();
}


void FOOTPRINT_WIZARD_FRAME::OnWizardParametersChanged()
{
    RegenerateFootprint();
    DisplayWizardInfos();
}

