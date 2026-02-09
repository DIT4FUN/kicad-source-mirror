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

#ifndef FOOTPRINT_WIZARD_H
#define FOOTPRINT_WIZARD_H

#include <tl/expected.hpp>

#include <api/common/types/wizards.pb.h>



// Wrapper for WizardMetaInfo protobuf with UTF8-converted strings
struct WIZARD_META_INFO
{
    wxString identifier;
    wxString name;
    wxString description;
    std::set<kiapi::common::types::WizardContentType> types_generated;

    void FromProto( const kiapi::common::types::WizardMetaInfo& aProto );
};

// Wrapper for WizardInfo protobuf
struct WIZARD_INFO
{
    WIZARD_META_INFO meta;
    std::vector<kiapi::common::types::WizardParameter> parameters;

    void FromProto( const kiapi::common::types::WizardInfo& aProto );
};


class FOOTPRINT_WIZARD
{
public:
    FOOTPRINT_WIZARD() {}
    ~FOOTPRINT_WIZARD() = default;

    WIZARD_INFO& Info() { return m_info; }
    const WIZARD_INFO& Info() const { return m_info; }

    const wxString& Identifier() const { return m_identifier; }
    void SetIdentifier( const wxString& aId ) { m_identifier = aId; }

private:
    WIZARD_INFO m_info;

    // Identifier of the plugin action
    wxString m_identifier;
};

/**
 * The footprint wizard manager interfaces with API_PLUGINs that can generate footprints.
 * It uses API_PLUGIN_MANAGER to enumerate the loaded wizard plugins, and filters to those that
 * generate footprints.  It then handles calling the plugin to query capabilities, generate
 * footprints, and so on.
 */
class FOOTPRINT_WIZARD_MANAGER
{
public:
    FOOTPRINT_WIZARD_MANAGER() {}
    ~FOOTPRINT_WIZARD_MANAGER() = default;

    void ReloadWizards();

    /**
     * Runs a wizard plugin with the --get-info argument, which should result in the plugin
     * dumping a WizardInfo protobuf message in JSON format to stdout.
     * @return true if the call succeeded
     */
    static bool RefreshInfo( FOOTPRINT_WIZARD& aWizard );

private:

    std::vector<FOOTPRINT_WIZARD> m_wizards;
};


#endif /* FOOTPRINT_WIZARD_H */
