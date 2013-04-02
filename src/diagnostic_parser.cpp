/**
 * @file   diagnostic_parser.cpp
 * @author Ugo Cupcic <ugo@shadowrobot.com>
 * @date   Wed Mar 27 06:26:23 2013
 *
 * Copyright 2011 Shadow Robot Company Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * @brief Parsing the diagnostics for jitter, dropped messages, etc...
 *
 *
 */

#include "sr_self_test/diagnostic_parser.hpp"

namespace shadow_robot
{
  DiagnosticParser::DiagnosticParser()
  {
    diagnostics_.push_back( new RTLoopDiagnostics("Realtime Control Loop"));

    diag_sub_ = nh_.subscribe("diagnostics_agg", 1, &DiagnosticParser::diagnostics_agg_cb_, this);
  }

  void DiagnosticParser::parse_diagnostics(diagnostic_updater::DiagnosticStatusWrapper& status)
  {
    for(size_t i=0; i<50; ++i)
    {      ros::Duration(0.1).sleep();
      ros::spinOnce();
    }

    std::pair<bool, std::string> res;
    bool ok = true;
    std::string full_parse_res = "Diagnostic parser: ";

    for(size_t i = 0; i < diagnostics_.size(); ++i)
    {
      res = diagnostics_[i].to_string();
      if( !res.first )
        ok = false;
      full_parse_res += res.second + " | ";
    }

    if(ok)
      status.summary( diagnostic_msgs::DiagnosticStatus::OK, full_parse_res );
    else
      status.summary( diagnostic_msgs::DiagnosticStatus::ERROR, full_parse_res );
  }

  void DiagnosticParser::diagnostics_agg_cb_(const diagnostic_msgs::DiagnosticArray::ConstPtr& msg)
  {
    for( size_t status_i = 0; status_i < msg->status.size(); ++status_i )
    {
      for( size_t diag_i = 0; diag_i < diagnostics_.size() ; ++diag_i )
      {
        if( msg->status[status_i].name.find(diagnostics_[diag_i].name) != std::string::npos )
        {
          diagnostics_[diag_i].parse_diagnostics(msg->status[status_i].values);
        }
      }
    }
  }
}

/* For the emacs weenies in the crowd.
   Local Variables:
   c-basic-offset: 2
   End:
*/

