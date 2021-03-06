#pragma once
#define _SCL_SECURE_NO_WARNINGS
#define NOMINMAX
#define COMPILER_MSVC

#ifndef TENSORFLOWCONFIG_H
#define TENSORFLOWCONFIG_H

#include <tensorflow/core/public/session.h>
#include <tensorflow/cc/ops/const_op.h>
#include <tensorflow/cc/ops/image_ops.h>
#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/core/framework/graph.pb.h>
#include <tensorflow/core/graph/default_device.h>
#include <tensorflow/core/graph/graph_def_builder.h>
#include <tensorflow/core/lib/core/threadpool.h>
#include <tensorflow/core/lib/io/path.h>
#include <tensorflow/core/lib/strings/stringprintf.h>
#include <tensorflow/core/platform/init_main.h>
#include <tensorflow/core/util/command_line_flags.h>

#endif