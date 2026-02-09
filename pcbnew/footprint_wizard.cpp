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

#include <api/api_plugin_manager.h>
#include <api/common/types/wizards.pb.h>
#include <pgm_base.h>

#include <google/protobuf/util/json_util.h>

#include "footprint_wizard.h"


bool FOOTPRINT_WIZARD_MANAGER::RefreshInfo( FOOTPRINT_WIZARD& aWizard )
{
    API_PLUGIN_MANAGER& manager = Pgm().GetPluginManager();

    wxString out, err;
    int ret = manager.InvokeActionSync( aWizard.Identifier(), { wxS( "--get-info" ) }, &out, &err );

    if( ret != 0 )
        return false;

    kiapi::common::types::WizardInfo info;

    google::protobuf::util::JsonParseOptions options;
    options.ignore_unknown_fields = true;

    if( !google::protobuf::util::JsonStringToMessage( out.ToStdString(), &info, options ).ok() )
        return false;

    aWizard.Info().FromProto( info );
    return true;
}


void WIZARD_META_INFO::FromProto( const kiapi::common::types::WizardMetaInfo& aProto )
{
    identifier = wxString::FromUTF8( aProto.identifier() );
    name = wxString::FromUTF8( aProto.name() );
    description = wxString::FromUTF8( aProto.description() );

    types_generated.clear();

    for( int type : aProto.types_generated() )
        types_generated.insert( static_cast<kiapi::common::types::WizardContentType>( type ) );
}


void WIZARD_INFO::FromProto( const kiapi::common::types::WizardInfo& aProto )
{
    meta.FromProto( aProto.meta() );
}
