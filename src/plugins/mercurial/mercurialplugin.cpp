#include "mercurialcontrol.h"
#include <QtPlugin>
static const VcsBaseEditorParameters editorParameters[] = {
    Constants::LOGAPP},
{   AnnotateOutput,
    Constants::ANNOTATEAPP},
{   DiffOutput,
    Constants::DIFFAPP}
static const VcsBaseSubmitEditorParameters submitEditorParameters = {
MercurialPlugin *MercurialPlugin::m_instance = nullptr;
MercurialPlugin::MercurialPlugin()
{
    m_instance = this;
}
    if (m_client) {
        delete m_client;
        m_client = nullptr;
    }

    m_instance = nullptr;
    Core::Context context(Constants::MERCURIAL_CONTEXT);
    m_client = new MercurialClient;
    auto vc = initializeVcs<MercurialControl>(context, m_client);
    new OptionsPage(vc, this);
    connect(m_client, &VcsBaseClient::changed, vc, &MercurialControl::changed);
    connect(m_client, &MercurialClient::needUpdate, this, &MercurialPlugin::update);
    const auto describeFunc = [this](const QString &source, const QString &id) {
        m_client->view(source, id);
    };
    const auto widgetCreator = []() { return new MercurialEditorWidget; };
    for (auto &editor : editorParameters)
        new VcsEditorFactory(&editor, widgetCreator, describeFunc, this);
    new VcsSubmitEditorFactory(&submitEditorParameters,
        []() { return new CommitEditor(&submitEditorParameters); }, this);

    return true;
void MercurialPlugin::createMenu(const Core::Context &context)
void MercurialPlugin::createFileActions(const Core::Context &context)
    connect(annotateFile, &QAction::triggered, this, &MercurialPlugin::annotateCurrentFile);
    connect(diffFile, &QAction::triggered, this, &MercurialPlugin::diffCurrentFile);
    connect(logFile, &QAction::triggered, this, &MercurialPlugin::logCurrentFile);
    connect(statusFile, &QAction::triggered, this, &MercurialPlugin::statusCurrentFile);
    connect(m_addAction, &QAction::triggered, this, &MercurialPlugin::addCurrentFile);
    connect(m_deleteAction, &QAction::triggered, this, &MercurialPlugin::promptToDeleteCurrentFile);
    connect(revertFile, &QAction::triggered, this, &MercurialPlugin::revertCurrentFile);
void MercurialPlugin::addCurrentFile()
    m_client->synchronousAdd(state.currentFileTopLevel(), state.relativeCurrentFile());
void MercurialPlugin::annotateCurrentFile()
    m_client->annotate(state.currentFileTopLevel(), state.relativeCurrentFile(), QString(), currentLine);
void MercurialPlugin::diffCurrentFile()
    m_client->diff(state.currentFileTopLevel(), QStringList(state.relativeCurrentFile()));
void MercurialPlugin::logCurrentFile()
    m_client->log(state.currentFileTopLevel(), QStringList(state.relativeCurrentFile()),
                  QStringList(), true);
void MercurialPlugin::revertCurrentFile()
    m_client->revertFile(state.currentFileTopLevel(), state.relativeCurrentFile(), reverter.revision());
void MercurialPlugin::statusCurrentFile()
    m_client->status(state.currentFileTopLevel(), state.relativeCurrentFile());
void MercurialPlugin::createDirectoryActions(const Core::Context &context)
    connect(action, &QAction::triggered, this, &MercurialPlugin::diffRepository);
    connect(action, &QAction::triggered, this, &MercurialPlugin::logRepository);
    connect(action, &QAction::triggered, this, &MercurialPlugin::revertMulti);
    connect(action, &QAction::triggered, this, &MercurialPlugin::statusMulti);
void MercurialPlugin::diffRepository()
    m_client->diff(state.topLevel());
void MercurialPlugin::logRepository()
    m_client->log(state.topLevel());
void MercurialPlugin::revertMulti()
    m_client->revertAll(state.topLevel(), reverter.revision());
void MercurialPlugin::statusMulti()
    m_client->status(state.topLevel());
void MercurialPlugin::createRepositoryActions(const Core::Context &context)
    connect(action, &QAction::triggered, this, &MercurialPlugin::pull);
    connect(action, &QAction::triggered, this, &MercurialPlugin::push);
    connect(action, &QAction::triggered, this, &MercurialPlugin::update);
    connect(action, &QAction::triggered, this, &MercurialPlugin::import);
    connect(action, &QAction::triggered, this, &MercurialPlugin::incoming);
    connect(action, &QAction::triggered, this, &MercurialPlugin::outgoing);
    connect(action, &QAction::triggered, this, &MercurialPlugin::commit);
    connect(m_createRepositoryAction, &QAction::triggered, this, &MercurialPlugin::createRepository);
void MercurialPlugin::pull()
    SrcDestDialog dialog(SrcDestDialog::incoming, Core::ICore::dialogParent());
    m_client->synchronousPull(dialog.workingDir(), dialog.getRepositoryString());
void MercurialPlugin::push()
    SrcDestDialog dialog(SrcDestDialog::outgoing, Core::ICore::dialogParent());
    m_client->synchronousPush(dialog.workingDir(), dialog.getRepositoryString());
void MercurialPlugin::update()
    m_client->update(state.topLevel(), updateDialog.revision());
void MercurialPlugin::import()
    m_client->import(state.topLevel(), fileNames);
void MercurialPlugin::incoming()
    SrcDestDialog dialog(SrcDestDialog::incoming, Core::ICore::dialogParent());
    m_client->incoming(state.topLevel(), dialog.getRepositoryString());
void MercurialPlugin::outgoing()
    m_client->outgoing(state.topLevel());
void MercurialPlugin::commit()
    connect(m_client, &MercurialClient::parsedStatus, this, &MercurialPlugin::showCommitWidget);
    m_client->emitParsedStatus(m_submitRepository);
void MercurialPlugin::showCommitWidget(const QList<VcsBaseClient::StatusItem> &status)
    disconnect(m_client, &MercurialClient::parsedStatus, this, &MercurialPlugin::showCommitWidget);
            this, &MercurialPlugin::diffFromEditorSelected);
    QString branch = versionControl()->vcsTopic(m_submitRepository);
                            m_client->settings().stringValue(MercurialSettings::userNameKey),
                            m_client->settings().stringValue(MercurialSettings::userEmailKey), status);
void MercurialPlugin::diffFromEditorSelected(const QStringList &files)
    m_client->diff(m_submitRepository, files);
void MercurialPlugin::commitFromEditor()
bool MercurialPlugin::submitEditorAboutToClose()
        m_client->commit(m_submitRepository, files, editorFile->filePath().toString(),
                         extraOptions);
void MercurialPlugin::updateActions(VcsBasePlugin::ActionState as)
    VcsBaseEditorWidget::testDiffFileResolving(editorParameters[2].id);
    VcsBaseEditorWidget::testLogResolving(editorParameters[0].id, data, "18473:692cbda1eb50", "18472:37100f30590f");