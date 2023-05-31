#include <juce_core/juce_core.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion::engine;

// Create an edit with a number of audio tracks and insert the given plugin on each.
std::unique_ptr<te::Edit> createBenchmarkEdit(te::Engine &engine, juce::String pluginXmlTypeName)
{
    std::unique_ptr<te::Edit> edit = te::Edit::createSingleTrackEdit(engine);

    const int trackCount = 200;
    for (int trackIndex = 0; trackIndex < trackCount; ++trackIndex)
    {
        auto audioTrack = edit->insertNewAudioTrack(te::TrackInsertPoint{nullptr, nullptr}, nullptr);

        auto plugin = edit->getPluginCache().createNewPlugin(pluginXmlTypeName, {});
        jassert(plugin.get() != nullptr);
        audioTrack->pluginList.insertPlugin(plugin, -1, nullptr);
    }

    return edit;
}

double runBenchmark(juce::String pluginXmlTypename)
{
    te::Engine engine("benchmark");

    auto edit = createBenchmarkEdit(engine, pluginXmlTypename);
    double time = 0.0;
    {
        juce::ScopedTimeMeasurement measurement(time);

        // Force a graph update immediately. In an real-world application this would be triggered by some change in the
        // edit and called from Edit::timerCallback.
        edit->getTransport().ensureContextAllocated(true);
    }

    return time;
}

int main(int argc, char *argv[])
{
    juce::ignoreUnused(argc, argv);
    juce::ScopedJuceInitialiser_GUI init;

    const juce::StringArray pluginTypesToTest{te::VolumeAndPanPlugin::xmlTypeName,
                                              te::AuxSendPlugin::xmlTypeName,
                                              te::AuxReturnPlugin::xmlTypeName,
                                              te::PatchBayPlugin::xmlTypeName};
    for (auto pluginType : pluginTypesToTest)
    {
        double time = runBenchmark(pluginType);
        juce::Logger::writeToLog(juce::String("[benchmark] ") + pluginType + juce::String(" = ") + juce::String(time));
    }

    return 0;
}