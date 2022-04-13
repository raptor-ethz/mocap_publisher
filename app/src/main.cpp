///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) OMG Plc 2009.
// All rights reserved.  This software is protected by copyright
// law and international treaties.  No part of this software / document
// may be reproduced or distributed in any form or by any means,
// whether transiently or incidentally to some other use of this software,
// without the written permission of the copyright owner.
//
///////////////////////////////////////////////////////////////////////////////

#include "include_helper.h"

constexpr static float x_offset = 0.5;
constexpr static float y_offset = 0.5;

int main(int argc, char *argv[])
{
  // Create participant. Arguments-> Domain id, QOS name
  DefaultParticipant dp(0, "mocap_publisher");

  if (argc > 11)
  {
    std::cout << "[ERROR] Mocap publisher only supports 10 objects. Change "
                 "code to allow for more."
              << std::endl;
    return 0;
  }

  std::vector<std::string> objects(10, "empty");

  // dummy topics for unused publishers
  std::vector<std::string> topics = {"empty0", "empty1", "empty2", "empty3",
                                     "empty4", "empty5", "empty6", "empty7",
                                     "empty8", "empty9"};
  std::vector<std::string> segment_topics = {"SegmentsEmpty0", "SegmentsEmpty1", "SegmentsEmpty2", "SegmentsEmpty3",
                                             "SegmentsEmpty4", "SegmentsEmpty5", "SegmentsEmpty6", "SegmentsEmpty7",
                                             "SegmentsEmpty8", "SegmentsEmpty9"};

  // load names from argument list
  for (int i = 1; i < argc; i++)
  {
    objects.at(i - 1) = argv[i]; // first argument is the command
    topics.at(i - 1) =
        "mocap_" + objects.at(i - 1); // first argument is the command
    segment_topics.at(i - 1) =
        "segments_" + objects.at(i - 1); // first argument is the command
  }
  // Print objects and topics (INFO)
  std::cout << "publishing the following objecst and topics:" << std::endl;
  for (int i = 0; i < argc - 1; i++)
  {
    std::cout << "object: \t" << objects.at(i) << "\t topic: \t" << topics.at(i)
              << std::endl;
  }
  for (int i = 0; i < argc - 1; i++)
  {
    std::cout << "object_segment: \t" << objects.at(i) << "\t topic: \t" << segment_topics.at(i)
              << std::endl;
  }

  // create mocap data publishers
  DDSPublisher pub[10] = {DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(0), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(1), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(2), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(3), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(4), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(5), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(6), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(7), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(8), dp.participant()),
                          DDSPublisher(idl_msg::Mocap_msgPubSubType(),
                                       topics.at(9), dp.participant())};
  // create mocap messages
  cpp_msg::Mocap_msg msg[10]{};

  // create segment data publishers
  DDSPublisher segment_pub[10] = {DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(0), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(1), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(2), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(3), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(4), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(5), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(6), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(7), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(8), dp.participant()),
                                  DDSPublisher(idl_msg::MocapSegments_msgPubSubType(),
                                               segment_topics.at(9), dp.participant())};
  // create segment messages
  cpp_msg::MocapSegments_msg segment_msg[10]{};

  // assign topic names to messages
  for (int i = 0; i < argc - 1; i++)
  {
    segment_msg[i].object_name = objects.at(i);
  }

  /* VICON Datastream Settings */
  std::vector<std::string> Hosts;
  // HARD CODE Vicon Address from LEO C6
  Hosts.push_back("10.10.10.5");
  if (Hosts.empty())
  {
    Hosts.push_back("localhost:801");
  }

  bool bOptimizeWireless = false;
  bool bQuiet = false;

  std::vector<std::string> HapticOnList(0);
  unsigned int ClientBufferSize = 0;
  std::string AxisMapping = "ZUp";
  std::vector<std::string> FilteredSubjects;
  std::vector<std::string> LocalAdapters;

  std::ostream &OutputStream(bQuiet ? NullStream : std::cout);

  bool First = true;
  std::string HostName;
  for (const auto &rHost : Hosts)
  {
    if (!First)
    {
      HostName += ";";
    }
    HostName += rHost;
    First = false;
  }

  // Make a new client
  ViconDataStreamSDK::CPP::Client DirectClient;

  if (bOptimizeWireless)
  {
    const Output_ConfigureWireless ConfigureWirelessResult =
        DirectClient.ConfigureWireless();

    if (ConfigureWirelessResult.Result != Result::Success)
    {
      std::cout << "Wireless Config: " << ConfigureWirelessResult.Error
                << std::endl;
    }
  }

  std::cout << "Connecting to " << HostName << " ..." << std::flush;
  while (!DirectClient.IsConnected().Connected)
  {
    // Direct connection
    const Output_Connect ConnectResult = DirectClient.Connect(HostName);
    const bool ok = (ConnectResult.Result == Result::Success);

    if (!ok)
    {
      std::cout << "Warning - connect failed... ";
      switch (ConnectResult.Result)
      {
      case Result::ClientAlreadyConnected:
        std::cout << "Client Already Connected" << std::endl;
        break;
      case Result::InvalidHostName:
        std::cout << "Invalid Host Name" << std::endl;
        break;
      case Result::ClientConnectionFailed:
        std::cout << "Client Connection Failed" << std::endl;
        break;
      default:
        std::cout << "Unrecognized Error: " << ConnectResult.Result
                  << std::endl;
        break;
      }
    }

    std::cout << ".";
#ifdef WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
    // }

    std::cout << std::endl;

    std::cout << std::endl;
    // Enable some different data types
    DirectClient.EnableSegmentData();

    DirectClient.SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ServerPush);
    // }

    // Set the global up axis
    DirectClient.SetAxisMapping(Direction::Forward, Direction::Left,
                                Direction::Up); // Z-up

    if (AxisMapping == "YUp")
    {
      DirectClient.SetAxisMapping(Direction::Forward, Direction::Up,
                                  Direction::Right); // Y-up
    }
    else if (AxisMapping == "XUp")
    {
      DirectClient.SetAxisMapping(Direction::Up, Direction::Forward,
                                  Direction::Left); // Y-up
    }

    Output_GetAxisMapping _Output_GetAxisMapping =
        DirectClient.GetAxisMapping();
    // std::cout << "Axis Mapping: X-" << Adapt(_Output_GetAxisMapping.XAxis) <<
    // " Y-" << Adapt(_Output_GetAxisMapping.YAxis) << " Z-"
    //          << Adapt(_Output_GetAxisMapping.ZAxis) << std::endl;

    // Discover the version number
    Output_GetVersion _Output_GetVersion = DirectClient.GetVersion();
    // std::cout << "Version: " << _Output_GetVersion.Major << "." <<
    // _Output_GetVersion.Minor << "." << _Output_GetVersion.Point << "."
    //           << _Output_GetVersion.Revision << std::endl;

    if (ClientBufferSize > 0)
    {
      DirectClient.SetBufferSize(ClientBufferSize);
      std::cout << "Setting client buffer size to " << ClientBufferSize
                << std::endl;
    }

    bool bSubjectFilterApplied = false;

    ViconDataStreamSDK::CPP::Client &MyClient(DirectClient);

    size_t Counter = 0;
    const std::chrono::high_resolution_clock::time_point StartTime =
        std::chrono::high_resolution_clock::now();
    // Loop until a key is pressed
#ifdef WIN32
    while (!Hit())
#else
    while (true)
#endif
    {
      // Get a frame
      // OutputStream << "Waiting for new frame...";
      while (MyClient.GetFrame().Result != Result::Success)
      {
// Sleep a little so that we don't lumber the CPU with a busy poll
#ifdef WIN32
        Sleep(200);
#else
        sleep(1);
#endif
      }
      if (!bSubjectFilterApplied)
      {
        for (const auto &rSubject : FilteredSubjects)
        {
          Output_AddToSubjectFilter SubjectFilterResult =
              MyClient.AddToSubjectFilter(rSubject);
          bSubjectFilterApplied = bSubjectFilterApplied ||
                                  SubjectFilterResult.Result == Result::Success;
        }
      }

      const std::chrono::high_resolution_clock::time_point Now =
          std::chrono::high_resolution_clock::now();

      /* GET FRAMENUMBER */
      for (int i = 0; i < argc - 1; i++)
      {
        msg[i].header.timestamp = MyClient.GetFrameNumber().FrameNumber;
      }

      /* GET LATENCY */
      for (int i = 0; i < argc - 1; i++)
      {
        msg[i].latency = MyClient.GetLatencyTotal().Total;
      }

      /* COUNT ALL SUBJECTS AND LOOP OVER THEM */
      unsigned int SubjectCount = MyClient.GetSubjectCount().SubjectCount;

      for (unsigned int SubjectIndex = 0; SubjectIndex < SubjectCount;
           ++SubjectIndex)
      {
        // Get the subject name
        std::string SubjectName = MyClient.GetSubjectName(SubjectIndex).SubjectName;
        // get subject index in pub array
        int pub_index = 0;
        for (int i = 0; i < argc - 1; i++)
        {
          if (SubjectName.compare(objects.at(i)) == 0)
          {
            pub_index = i;
          }
        }
        msg[pub_index].header.description = SubjectName;
        segment_msg[pub_index].object_name = SubjectName;

        /* COUNT ALL SEGMENTS AND LOOP OVER THEM (i think there is usually only one segment) */
        unsigned int SegmentCount = MyClient.GetSegmentCount(SubjectName).SegmentCount;

        for (unsigned int SegmentIndex = 0; SegmentIndex < SegmentCount;
             ++SegmentIndex)
        {

          /* GLOBAL SEGMENT NAME */
          std::string SegmentName =
              MyClient.GetSegmentName(SubjectName, SegmentIndex).SegmentName;

          /* GLOBAL OBJECT TRANSLATION */
          Output_GetSegmentGlobalTranslation
              _Output_GetSegmentGlobalTranslation =
                  MyClient.GetSegmentGlobalTranslation(SubjectName,
                                                       SegmentName);
          msg[pub_index]
              .position.x =
              (_Output_GetSegmentGlobalTranslation.Translation[0] /
               1000.0) -
              x_offset;
          msg[pub_index].position.y =
              ((_Output_GetSegmentGlobalTranslation.Translation[1] /
                1000.0) +
               y_offset) *
              (-1.0);
          msg[pub_index].position.z =
              _Output_GetSegmentGlobalTranslation.Translation[2] / 1000.0;

          /* GLOBAL OBJECT ORIENTATION [EULER ANGLES] */
          Output_GetSegmentGlobalRotationEulerXYZ
              _Output_GetSegmentGlobalRotationEulerXYZ =
                  MyClient.GetSegmentGlobalRotationEulerXYZ(SubjectName,
                                                            SegmentName);

          msg[pub_index].orientation.roll =
              _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[0] *
              (180.0 / M_PI);
          msg[pub_index].orientation.pitch =
              _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[1] *
              (180.0 / M_PI);
          msg[pub_index].orientation.yaw =
              _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[2] *
              (180.0 / M_PI);

          /* GLOBAL OBJECT ORIENTATION [QUATERNIONS] (not supported as of now)*/
          // Output_GetSegmentGlobalRotationQuaternion
          //     _Output_GetSegmentGlobalRotationQuaternion =
          //         MyClient.GetSegmentGlobalRotationQuaternion(SubjectName,
          //                                                     SegmentName);
        }

        /* COUNT ALL MARKERS AND LOOP OVER THEM */
        unsigned int MarkerCount = MyClient.GetMarkerCount(SubjectName).MarkerCount;
        segment_msg[pub_index].length = MarkerCount;
        // check if markers count is too high for message type
        if (MarkerCount > 10)
        {
          std::cout << "too many markers (more than 10). Current fastDDS message only supports 10 markers per object" << std::endl;
        }
        for (unsigned int MarkerIndex = 0; MarkerIndex < MarkerCount;
             ++MarkerIndex)
        {
          // get marker name
          std::string MarkerName = MyClient.GetMarkerName(SubjectName, MarkerIndex).MarkerName;

          /* GLOBAL MARKER TRANSLATION */
          Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation = MyClient.GetMarkerGlobalTranslation(SubjectName, MarkerName);
          segment_msg[pub_index].segment_x[MarkerIndex] = _Output_GetMarkerGlobalTranslation.Translation[0];
          segment_msg[pub_index].segment_x[MarkerIndex] = _Output_GetMarkerGlobalTranslation.Translation[1];
          segment_msg[pub_index].segment_x[MarkerIndex] = _Output_GetMarkerGlobalTranslation.Translation[2];
        }
        /* PUBLISH MESSAGES */
        pub[pub_index].publish(msg[pub_index]);
        // segment_pub[i].publish(segment_msg[i]);
      }
    }

    ++Counter;
  }
}

/*

          if (SegmentCount > 10)
          {
            std::cout << "too many segments for Message" << std::endl;
          }
          if (SegmentIndex < 10)
          {
            segment_msg[pub_index].segment_x[SegmentIndex] = _Output_GetSegmentGlobalTranslation.Translation[1];
            segment_msg[pub_index].segment_y[SegmentIndex] = _Output_GetSegmentGlobalTranslation.Translation[2];
            segment_msg[pub_index].segment_z[SegmentIndex] = _Output_GetSegmentGlobalTranslation.Translation[3];
          }



        */