/***************************************************************************
 *   Copyright (C) 2019 PCSX-Redux authors                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.           *
 ***************************************************************************/

#include "core/sstate.h"

#include "core/callstacks.h"
#include "core/cdrom.h"
#include "core/gpu.h"
#include "core/mdec.h"
#include "core/psxcounters.h"
#include "core/psxemulator.h"
#include "core/psxmem.h"
#include "core/r3000a.h"
#include "core/sio.h"
#include "spu/interface.h"

PCSX::SaveStates::SaveState PCSX::SaveStates::constructSaveState() {
    // clang-format off
    return SaveState {
        SaveStateInfo {
            VersionString {},
            Version {},
        },
        Thumbnail {},
        Memory {
            RAM { g_emulator->m_psxMem->g_psxM },
            ROM { g_emulator->m_psxMem->g_psxR },
            Parallel { g_emulator->m_psxMem->g_psxP },
            HardwareMemory { g_emulator->m_psxMem->g_psxH },
        },
        Registers {
            GPR { g_emulator->m_psxCpu->m_psxRegs.GPR.r },
            CP0 { g_emulator->m_psxCpu->m_psxRegs.CP0.r },
            CP2D { g_emulator->m_psxCpu->m_psxRegs.CP2D.r },
            CP2C { g_emulator->m_psxCpu->m_psxRegs.CP2C.r },
            PC { g_emulator->m_psxCpu->m_psxRegs.pc },
            Code { g_emulator->m_psxCpu->m_psxRegs.code },
            Cycle { g_emulator->m_psxCpu->m_psxRegs.cycle },
            Interrupt { g_emulator->m_psxCpu->m_psxRegs.interrupt },
            ICacheAddr { g_emulator->m_psxCpu->m_psxRegs.ICache_Addr },
            ICacheCode { g_emulator->m_psxCpu->m_psxRegs.ICache_Code },
            NextIsDelaySlot { g_emulator->m_psxCpu->m_nextIsDelaySlot },
            DelaySlotInfo1 {
                DelaySlotIndex { g_emulator->m_psxCpu->m_delayedLoadInfo[0].index },
                DelaySlotValue { g_emulator->m_psxCpu->m_delayedLoadInfo[0].value },
                DelaySlotMask { g_emulator->m_psxCpu->m_delayedLoadInfo[0].mask },
                DelaySlotPcValue { g_emulator->m_psxCpu->m_delayedLoadInfo[0].pcValue },
                DelaySlotActive { g_emulator->m_psxCpu->m_delayedLoadInfo[0].active },
                DelaySlotPcActive { g_emulator->m_psxCpu->m_delayedLoadInfo[0].pcActive },
                DelaySlotFromLink { g_emulator->m_psxCpu->m_delayedLoadInfo[0].fromLink }
            },
            DelaySlotInfo2 {
                DelaySlotIndex { g_emulator->m_psxCpu->m_delayedLoadInfo[1].index },
                DelaySlotValue { g_emulator->m_psxCpu->m_delayedLoadInfo[1].value },
                DelaySlotMask { g_emulator->m_psxCpu->m_delayedLoadInfo[1].mask },
                DelaySlotPcValue { g_emulator->m_psxCpu->m_delayedLoadInfo[1].pcValue },
                DelaySlotActive { g_emulator->m_psxCpu->m_delayedLoadInfo[1].active },
                DelaySlotPcActive { g_emulator->m_psxCpu->m_delayedLoadInfo[1].pcActive },
                DelaySlotFromLink { g_emulator->m_psxCpu->m_delayedLoadInfo[1].fromLink }
            },
            CurrentDelayedLoad { g_emulator->m_psxCpu->m_currentDelayedLoad },
            IntTargetsField { g_emulator->m_psxCpu->m_psxRegs.intTargets },
            InISR { g_emulator->m_psxCpu->m_inISR },
        },
        GPU {},
        SPU {},
        SIO {
            SIOBuffer { g_emulator->m_sio->m_buffer },
            SIOStatusReg { g_emulator->m_sio->m_statusReg },
            SIOModeReg { g_emulator->m_sio->m_modeReg },
            SIOCtrlReg { g_emulator->m_sio->m_ctrlReg },
            SIOBaudReg { g_emulator->m_sio->m_baudReg },
            SIOBufferMaxIndex { g_emulator->m_sio->m_maxBufferIndex },
            SIOBufferIndex { g_emulator->m_sio->m_bufferIndex },
            SIOMCDState { g_emulator->m_sio->m_mcdState },
            SIOMCDReadWriteState { g_emulator->m_sio->m_mcdReadWriteState },
            SIOMCDAddrHigh { g_emulator->m_sio->m_mcdAddrHigh },
            SIOMCDAddrLow { g_emulator->m_sio->m_mcdAddrLow },
            SIOPadState { g_emulator->m_sio->m_padState },
            SIOWasMCD1Inserted { g_emulator->m_sio->m_wasMcd1Inserted },
            SIOWasMCD2Inserted { g_emulator->m_sio->m_wasMcd2Inserted },
        },
        CDRom {
            CDOCUP { g_emulator->m_cdrom->m_OCUP },
            CDReg1Mode { g_emulator->m_cdrom->m_reg1Mode },
            CDReg2 { g_emulator->m_cdrom->m_reg2 },
            CDCmdProcess { g_emulator->m_cdrom->m_cmdProcess },
            CDCtrl { g_emulator->m_cdrom->m_ctrl },
            CDStat { g_emulator->m_cdrom->m_stat },
            CDStatP { g_emulator->m_cdrom->m_statP },
            CDTransfer { reinterpret_cast<uint8_t*>(g_emulator->m_cdrom->m_transfer) },
            CDTransferIndex { g_emulator->m_cdrom->m_transferIndex },
            CDPrev { g_emulator->m_cdrom->m_prev },
            CDParam { g_emulator->m_cdrom->m_param },
            CDResult { g_emulator->m_cdrom->m_result },
            CDParamC { g_emulator->m_cdrom->m_paramC },
            CDParamP { g_emulator->m_cdrom->m_paramP },
            CDResultC { g_emulator->m_cdrom->m_resultC },
            CDResultP { g_emulator->m_cdrom->m_resultP },
            CDResultReady { g_emulator->m_cdrom->m_resultReady },
            CDCmd { g_emulator->m_cdrom->m_cmd },
            CDRead { g_emulator->m_cdrom->m_read },
            CDSetLocPending { g_emulator->m_cdrom->m_setlocPending },
            CDReading { g_emulator->m_cdrom->m_reading },
            CDResultTN { g_emulator->m_cdrom->m_resultTN },
            CDResultTD { g_emulator->m_cdrom->m_resultTD },
            CDSetSectorPlay { g_emulator->m_cdrom->m_setSectorPlay },
            CDSetSectorEnd { g_emulator->m_cdrom->m_setSectorEnd },
            CDSetSector { g_emulator->m_cdrom->m_setSector },
            CDTrack { g_emulator->m_cdrom->m_track },
            CDPlay { g_emulator->m_cdrom->m_play },
            CDMuted { g_emulator->m_cdrom->m_muted },
            CDCurTrack { g_emulator->m_cdrom->m_curTrack },
            CDMode { g_emulator->m_cdrom->m_mode },
            CDFile { g_emulator->m_cdrom->m_file },
            CDChannel { g_emulator->m_cdrom->m_channel },
            CDSuceeded { g_emulator->m_cdrom->m_suceeded },
            CDFirstSector { g_emulator->m_cdrom->m_firstSector },
            CDIRQ { g_emulator->m_cdrom->m_irq },
            CDIrqRepeated { g_emulator->m_cdrom->m_irqRepeated },
            CDECycle { g_emulator->m_cdrom->m_eCycle },
            CDSeeked { g_emulator->m_cdrom->m_seeked },
            CDReadRescheduled { g_emulator->m_cdrom->m_readRescheduled },
            CDDriveState { g_emulator->m_cdrom->m_driveState },
            CDFastForward { g_emulator->m_cdrom->m_fastForward },
            CDFastBackward { g_emulator->m_cdrom->m_fastBackward },
            CDAttenuatorLeftToLeft { g_emulator->m_cdrom->m_attenuatorLeftToLeft },
            CDAttenuatorLeftToRight { g_emulator->m_cdrom->m_attenuatorLeftToRight },
            CDAttenuatorRightToRight { g_emulator->m_cdrom->m_attenuatorRightToRight },
            CDAttenuatorRightToLeft { g_emulator->m_cdrom->m_attenuatorRightToLeft },
            CDAttenuatorLeftToLeftT { g_emulator->m_cdrom->m_attenuatorLeftToLeftT },
            CDAttenuatorLeftToRightT { g_emulator->m_cdrom->m_attenuatorLeftToRightT },
            CDAttenuatorRightToRightT { g_emulator->m_cdrom->m_attenuatorRightToRightT },
            CDAttenuatorRightToLeftT { g_emulator->m_cdrom->m_attenuatorRightToLeftT },
            CDSubQTrack { g_emulator->m_cdrom->m_subq.track },
            CDSubQIndex { g_emulator->m_cdrom->m_subq.index },
            CDSubQRelative { g_emulator->m_cdrom->m_subq.relative },
            CDSubQAbsolute { g_emulator->m_cdrom->m_subq.absolute },
            CDTrackChanged { g_emulator->m_cdrom->m_trackChanged },
            CDLocationChanged { g_emulator->m_cdrom->m_locationChanged },
        },
        Hardware {},
        Counters {},
        MDEC {},
        PCdrvFilesField {},
        CallStacks {},
    };
    // clang-format on
}

namespace PCSX {
struct SaveStateWrapper {
    SaveStateWrapper(SaveStates::SaveState& state_) : state(state_) {}
    SaveStates::SaveState& state;
};
}  // namespace PCSX

std::string PCSX::SaveStates::save() {
    SaveState state = constructSaveState();
    SaveStateWrapper wrapper(state);

    state.get<SaveStateInfoField>().get<VersionString>().value = "PCSX-Redux SaveState v2";
    state.get<SaveStateInfoField>().get<Version>().value = 2;

    g_emulator->m_gpu->save(state.get<GPUField>());
    g_emulator->m_spu->save(state.get<SPUField>());

    g_emulator->m_psxCounters->save(state.get<CountersField>());
    g_emulator->m_mdec->save(state.get<MDECField>());

    g_emulator->m_psxCpu->listAllPCdevFiles([&state](uint16_t fd, std::filesystem::path filename, bool create) {
        state.get<PCdrvFilesField>().value.emplace_back(fd, filename.string(), create);
    });

    g_emulator->m_callStacks->serialize(&wrapper);

    Protobuf::OutSlice slice;
    state.serialize(&slice);
    return slice.finalize();
}

void PCSX::CallStacks::serialize(SaveStateWrapper* w) {
    using namespace SaveStates;
    auto& callstacks = w->state.get<SaveStates::CallStacksField>().get<CallStacksMessageField>().value;
    for (auto& callstack : getCallstacks()) {
        SaveStates::CallStack sscallstack{};
        sscallstack.get<LowSP>().value = callstack.getLow();
        sscallstack.get<HighSP>().value = callstack.getHigh();
        sscallstack.get<PresumedRA>().value = callstack.ra;
        sscallstack.get<PresumedFP>().value = callstack.fp;
        sscallstack.get<CallstackIsCurrent>().value = &callstack == m_current;
        for (auto& call : callstack.calls) {
            sscallstack.get<Calls>().value.emplace_back(call.ra, call.sp, call.fp, call.shadow);
        }
        callstacks.emplace_back(sscallstack);
    }
    w->state.get<SaveStates::CallStacksField>().get<CallStacksCurrentSP>().value = m_currentSP;
}

bool PCSX::SaveStates::load(const std::string& data) {
    SaveState state = constructSaveState();

    Protobuf::InSlice slice(reinterpret_cast<const uint8_t*>(data.data()), data.size());
    try {
        state.deserialize(&slice, 0);
    } catch (...) {
        return false;
    }

    if (state.get<SaveStateInfoField>().get<Version>().value != 2) {
        return false;
    }

    SaveStateWrapper wrapper(state);
    PCSX::g_emulator->m_psxCpu->Reset();
    state.commit();
    g_emulator->m_psxCpu->m_psxRegs.lowestTarget = g_emulator->m_psxCpu->m_psxRegs.cycle;
    g_emulator->m_psxCpu->m_psxRegs.previousCycles = g_emulator->m_psxCpu->m_psxRegs.cycle;
    g_emulator->m_gpu->load(state.get<GPUField>());
    g_emulator->m_spu->load(state.get<SPUField>());
    g_emulator->m_cdrom->load();

    g_emulator->m_psxCounters->load(state.get<CountersField>());
    g_emulator->m_mdec->load(state.get<MDECField>());

    auto& xa = state.get<SPUField>().get<SaveStates::XAField>();

    g_emulator->m_cdrom->m_xa.freq = xa.get<SaveStates::XAFrequency>().value;
    g_emulator->m_cdrom->m_xa.nbits = xa.get<SaveStates::XANBits>().value;
    g_emulator->m_cdrom->m_xa.nsamples = xa.get<SaveStates::XANSamples>().value;
    g_emulator->m_cdrom->m_xa.stereo = xa.get<SaveStates::XAStereo>().value;
    auto& left = xa.get<SaveStates::XAADPCMLeft>();
    g_emulator->m_cdrom->m_xa.left.y0 = left.get<SaveStates::ADPCMDecodeY0>().value;
    g_emulator->m_cdrom->m_xa.left.y1 = left.get<SaveStates::ADPCMDecodeY1>().value;
    auto& right = xa.get<SaveStates::XAADPCMLeft>();
    g_emulator->m_cdrom->m_xa.right.y0 = right.get<SaveStates::ADPCMDecodeY0>().value;
    g_emulator->m_cdrom->m_xa.right.y1 = right.get<SaveStates::ADPCMDecodeY1>().value;
    xa.get<SaveStates::XAPCM>().copyTo(reinterpret_cast<uint8_t*>(g_emulator->m_cdrom->m_xa.pcm));
    g_emulator->m_spu->playADPCMchannel(&g_emulator->m_cdrom->m_xa);

    g_emulator->m_psxCpu->closeAllPCdevFiles();
    for (auto& file : state.get<PCdrvFilesField>().value) {
        uint16_t fd = file.get<PCdrvFD>().value;
        std::string filename = file.get<PCdrvFilename>().value;
        bool create = file.get<PCdrvCreate>().value;
        if (create) {
            g_emulator->m_psxCpu->restorePCdrvFile(filename, fd, File::CREATE);
        } else {
            g_emulator->m_psxCpu->restorePCdrvFile(filename, fd);
        }
    }
    g_emulator->m_callStacks->deserialize(&wrapper);

    return true;
}

void PCSX::CallStacks::deserialize(const SaveStateWrapper* w) {
    using namespace SaveStates;
    m_callstacks.destroyAll();

    auto& callstacks = w->state.get<SaveStates::CallStacksField>().get<CallStacksMessageField>().value;
    m_current = nullptr;

    for (auto& sscallstack : callstacks) {
        auto& calls = sscallstack.get<Calls>().value;
        uint32_t lowSP = sscallstack.get<LowSP>().value;
        uint32_t highSP = sscallstack.get<HighSP>().value;
        uint32_t ra = sscallstack.get<PresumedRA>().value;
        uint32_t fp = sscallstack.get<PresumedFP>().value;
        bool isCurrent = sscallstack.get<CallstackIsCurrent>().value;
        CallStack* callstack = new CallStack();
        callstack->ra = ra;
        callstack->fp = fp;
        for (auto& call : calls) {
            uint32_t ra = call.get<CallRA>().value;
            uint32_t sp = call.get<CallSP>().value;
            uint32_t fp = call.get<CallFP>().value;
            bool shadow = call.get<Shadow>().value;
            callstack->calls.push_back(new CallStack::Call(sp, fp, ra, shadow));
        }
        if (isCurrent) m_current = callstack;
        m_callstacks.insert(lowSP, highSP, callstack);
    }

    m_currentSP = w->state.get<SaveStates::CallStacksField>().get<CallStacksCurrentSP>().value;
}
