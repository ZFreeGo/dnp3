
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "TransportLoopbackTestObject.h"

#include <DNP3/LinkRoute.h>

#include <sstream>


using namespace std;

namespace apl
{
namespace dnp
{

TransportLoopbackTestObject::TransportLoopbackTestObject(
    boost::asio::io_service* apService,
    IPhysicalLayerAsync* apPhys,
    LinkConfig aCfgA,
    LinkConfig aCfgB,
    FilterLevel aLevel,
    bool aImmediate) :

	LogTester(aImmediate),
	AsyncTestObjectASIO(apService),
	mpLogger(mLog.GetLogger(aLevel, "test")),
	mStrand(*this->GetService()),
	mExecutor(&mStrand),
	mCfgA(aCfgA),
	mCfgB(aCfgB),
	mLinkA(mpLogger, &mExecutor, aCfgA),
	mLinkB(mpLogger, &mExecutor, aCfgB),
	mTransA(mpLogger),
	mTransB(mpLogger),
	mRouter(mpLogger, apPhys, &mExecutor, 1000),
	mUpperA(mpLogger),
	mUpperB(mpLogger)
{
	mRouter.AddContext(&mLinkA, LinkRoute(mCfgA.RemoteAddr, mCfgA.LocalAddr));
	mRouter.AddContext(&mLinkB, LinkRoute(mCfgB.RemoteAddr, mCfgB.LocalAddr));

	mLinkA.SetUpperLayer(&mTransA);
	mLinkB.SetUpperLayer(&mTransB);

	mLinkA.SetRouter(&mRouter);
	mLinkB.SetRouter(&mRouter);

	mTransA.SetUpperLayer(&mUpperA);
	mTransB.SetUpperLayer(&mUpperB);
}

TransportLoopbackTestObject::~TransportLoopbackTestObject()
{
	mRouter.Shutdown();
}

bool TransportLoopbackTestObject::LayersUp()
{
	return mUpperA.IsLowerLayerUp() && mUpperB.IsLowerLayerUp();
}

void TransportLoopbackTestObject::Start()
{
	mRouter.Start();
}

}
}


